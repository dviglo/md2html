// cmark-gfm
#include <../extensions/cmark-gfm-core-extensions.h>
#include <parser.h>
#include <registry.h>

#include "Utils.h"
#include "Utf8Str.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>


using namespace std;
namespace fs = std::filesystem;


// Запись в таблице тег -> список статей
struct Article
{
    string title_;
    string url_; // Относительно корневого каталога

    // Использует при сортировке статей
    bool operator <(const Article& rhs) const
    {
        return title_ < rhs.title_;
    }
};

// Тег для статей без тегов
const string NO_TAG = "нет";

// Словарь тег -> список статей
unordered_map<string, vector<Article>> _tagMap;

// Статьи без тегов
vector<Article> _withoutTags;

// Уже нормализованы
string _inputRootDirPath;
string _outputRootDirPath;


// Подключает все расширения к парсеру Markdown
void AttachAllExtensions(cmark_parser* parser)
{
    cmark_mem* mem = cmark_get_default_mem_allocator();
    cmark_llist* syntax_extensions = cmark_list_syntax_extensions(mem);

    for (cmark_llist* i = syntax_extensions; i; i = i->next)
    {
        cmark_syntax_extension* extension = (cmark_syntax_extension*)i->data;
        cmark_parser_attach_syntax_extension(parser, extension);
    }

    cmark_llist_free(mem, syntax_extensions);
}


string AddHtmlStuff(const string& body, int depth, bool index_html = false)
{
    string rootPath;
    for (int i = 0; i < depth; i++)
        rootPath += "../";

    string result =
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "<meta charset='utf-8'>\n"
        "<link rel='stylesheet' href='" + rootPath + "___res/KaTeX/katex.min.css'>\n"
        "<link rel='stylesheet' href='" + rootPath + "___res/KaTeX/contrib/copy-tex.min.css'>\n"
        "<link rel='stylesheet' href='" + rootPath + "___res/Styles.css'>\n"
        "</head>\n"

        "<body onload='renderMathInElement(document.body);'>\n"

        "<script>\n"
        "let nestingLevel = " + std::to_string(depth) + ";\n"
        "</script>\n"

        "<script defer src='" + rootPath + "___res/UnprotectMath.js'></script>\n"
        "<script defer src='" + rootPath + "___res/KaTeX/katex.min.js'></script>\n"
        "<script defer src='" + rootPath + "___res/KaTeX/contrib/auto-render.min.js'></script>\n"
        "<script defer src='" + rootPath + "___res/KaTeX/contrib/copy-tex.min.js'></script>\n"
        "<script defer src='" + rootPath + "___res/TagTable.js'></script>\n"
        "<script defer src='" + rootPath + "___res/Utils.js'></script>\n";

    if (index_html)
        result += "<script defer src='" + rootPath + "___res/Index.js'></script>\n";
    else
        result += "<script defer src='" + rootPath + "___res/Article.js'></script>\n";

    result +=
        "<script defer src='" + rootPath + "___res/Addon.js'></script>\n"
        
        + body +
        
        "</body>\n"
        "</html>\n";

    return result;
}


string MarkdownToHtmlBody(const string& markdown)
{
    const int OPTIONS = CMARK_OPT_VALIDATE_UTF8 | CMARK_OPT_UNSAFE;
    cmark_parser* parser = cmark_parser_new(OPTIONS);
    AttachAllExtensions(parser);
    cmark_parser_feed(parser, markdown.c_str(), markdown.length());
    cmark_node* doc = cmark_parser_finish(parser);
    char* htmlBody = cmark_render_html(doc, OPTIONS, parser->syntax_extensions);
    cmark_parser_free(parser);
    cmark_node_free(doc);
    string result = htmlBody;
    free(htmlBody);
    return result;
}


string ExtractTitle(string markdown)
{
    markdown = ReplaceAll(markdown, "\r\n", "\n");
    markdown = ReplaceAll(markdown, "\r", "\n");

    smatch match;
    // . не соответствует переводу строки, поэтому используется [\s\S]
    regex_match(markdown, match, regex(R"(# ([^\n]+)[\s\S]*)"));
    if (!match.empty())
        return Trim(match[1].str());

    return string();
}


vector<string> ExtractTags(string markdown)
{
    markdown = ReplaceAll(markdown, "\r\n", "\n");
    markdown = ReplaceAll(markdown, "\r", "\n");

    vector<string> result;
    smatch match;

    regex_match(markdown, match, regex(R"([\s\S]*\*\*Теги:([^\n]+)\*\*\s*$)"));

    if (!match.empty())
    {
        result = Split(match[1].str(), ',');

        for (int i = 0; i < result.size(); i++)
        {
            string tag = Trim(result[i]);
            result[i] = ToLower(tag);
        }
    }

    return result;
}


// Пополняет словарь тег -> список статей. Пытается извлечь заголовок из документа,
// а если это не получается, то использует название файла
void AddToTable(const string& fileName, const string& relativeFilePath, const string& markdown, int depth)
{
    vector<string> tags = ExtractTags(markdown);
    string title = ExtractTitle(markdown);
    if (title.empty())
        title = CutEnd(fileName, ".md");

    string url = ReplaceAll(relativeFilePath, " ", "%20");
    // TODO: заменить кавычки и двойные кавычки в путях, а то js-скрипт поломается (или экранировать?)
    url += ".html"; // После .md дописываем .html, так как Хром не хочет открывать md-файлы как веб-странички

    Article article;
    article.title_ = title;
    article.url_ = url;

    for (const string& tag : tags)
        _tagMap[tag].push_back(article);

    if (!tags.size())
        _withoutTags.push_back(article);
}


void SaveTagTable()
{
    fs::create_directories(_outputRootDirPath + "/___res");
    ofstream ofs(_outputRootDirPath + "/___res/TagTable.js");
    ofs <<
        "// Этот файл генерируется утилитой md2html\n"
        "\n"
        "class Article\n"
        "{\n"
        "\tconstructor(title, url)\n"
        "\t{\n"
        "\t\tthis.title = title;\n"
        "\t\tthis.url = url;\n"
        "\t}\n"
        "}\n"
        "\n"
        "// Хеш-таблица тег -> массив статей\n"
        "\n"
        "let tagTable = {\n";

    // Первыми будут статьи без тегов
    if (_withoutTags.size())
    {
        sort(_withoutTags.begin(), _withoutTags.end());
        ofs << "\t'" << NO_TAG << "' : [";

        for (const Article& article : _withoutTags)
            ofs << "new Article('" << article.title_ << "', '" << article.url_ << "'), ";

        ofs << "],\n";
    }

    if (_tagMap.size())
    {
        // Сортируем по тегу
        vector<string> tags;
        tags.reserve(_tagMap.size());
        for (auto& it : _tagMap)
            tags.push_back(it.first);
        sort(tags.begin(), tags.end());

        for (const string& tag : tags)
        {
            vector<Article>& articles = _tagMap[tag];
            sort(articles.begin(), articles.end());
            
            ofs << "\t'" + tag + "' : [";

            for (const Article& article : articles)
                ofs << "new Article('" << article.title_ << "', '" << article.url_ << "'), ";

            ofs << "],\n";
        }
    }

    ofs << "};\n";
}


// dir должна быть нормализована. depth - глубина вложенности папки относительно начальной папки 
void ProcessFilesRecursively(const string& dirPath, int depth)
{
    vector<string> files;
    vector<string> subdirs;
    ReadDir(dirPath, files, subdirs);

    for (const string& fileName : files)
    {
        string relativeDirPath = CutStart(dirPath, _inputRootDirPath);
        if (StartsWith(relativeDirPath, "/"))
            relativeDirPath = CutStart(relativeDirPath, "/");
        string outputDirPath = _outputRootDirPath + '/' + relativeDirPath;
        fs::create_directories(outputDirPath);
        string relativeFilePath = relativeDirPath.empty() ? fileName : relativeDirPath + '/' + fileName;
        string inputFilePath = dirPath + '/' + fileName;
        string outputFilePath = outputDirPath + '/' + fileName;

        if (EndsWith(fileName, ".md"))
        {
            string markdown = ReadAllText(inputFilePath);
            string htmlBody = MarkdownToHtmlBody(markdown);
            string html = AddHtmlStuff(htmlBody, depth);

            // После .md дописываем .html, так как Хром не хочет открывать md-файлы как веб-странички
            WriteAllText(outputFilePath + ".html", html);

            AddToTable(fileName, relativeFilePath, markdown, depth);
        }
        else
        {
            fs::copy_file(inputFilePath, outputFilePath, fs::copy_options::overwrite_existing);
        }
    }

    for (const string& subdir : subdirs)
        ProcessFilesRecursively(dirPath + '/' + subdir, depth + 1);
}


void SaveIndexHtml()
{
    ofstream ofs(_outputRootDirPath + "/index.html");
    ofs << AddHtmlStuff("", 0, true); // У index.html тело формируется джава-скриптом на лету
}


int main(int argc, char* argv[])
{
    // Нам нужны имена файлов в кодировке UTF-8.
    // Требуется в винде, а в линуске похоже и без этого работает
    locale::global(locale("en_US.UTF-8"));

    if (argc != 3)
    {
        cout << "Usage: md2html <inputDir> <outputDir>";
        return 0;
    }

#ifdef _WIN32
    _inputRootDirPath = NormalizePath(cp1251_to_utf8(argv[1]));
    cout << "ВХОД: " << _inputRootDirPath << "\n";

    _outputRootDirPath = NormalizePath(cp1251_to_utf8(argv[2]));
    cout << "ВЫХОД: " << _outputRootDirPath << "\n";
#else
    _inputRootDirPath = NormalizePath(argv[1]);
    cout << "ВХОД: " << _inputRootDirPath << "\n";

    _outputRootDirPath = NormalizePath(argv[2]);
    cout << "ВЫХОД: " << _outputRootDirPath << "\n";
#endif

    if (!fs::exists(_inputRootDirPath))
    {
        cout << "Входная директория не найдена!\n";
        return 1;
    }

    // Регистрируем плагины cmark-gfm
    cmark_gfm_core_extensions_ensure_registered();
    
    ProcessFilesRecursively(_inputRootDirPath, 0);
    SaveTagTable();
    SaveIndexHtml();

    // Уничтожаем плагины
    cmark_release_plugins();

    return 0;
}
