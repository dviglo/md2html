/*
    Чтобы cmark при преобразовании md -> html не пытался парсить формулы,
    юзер должен оформлять их как `inline code`.
    Когда браузер открывает html-файл, этот скрипт извлекает формулы
    из <code></code>, чтобы KaTeX их не игнорировал.
    Источник: https://yihui.org/en/2018/07/latex-math-markdown/
*/

{
    let codes = document.getElementsByTagName("code");

    for (let i = 0; i < codes.length;)
    {
        let code = codes[i];

        if (code.parentNode.tagName !== "PRE" && code.childElementCount === 0)
        {
            let content = code.textContent;

            // Если содержимое начинается с одиночного $ и заканчивается одиночным $
            if (/^\s*\$[^$]/.test(content) && /[^$]\$\s*$/.test(content))
            {
                // Заменяем ограничители на \( и \)
                content = content.replace(/^\s*\$/, "\\(").replace(/\$\s*$/, "\\)");
                code.textContent = content;
            }

            if (/^\s*\$\$[\s\S]+\$\$\s*$/.test(content) // Если начинается с $$ и заканчивается $$
                || /^\s*\\\([\s\S]+\\\)\s*$/.test(content) // или начинается с \( и заканчивается \)
                || /^\s*\\\[[\s\S]+\\\]\s*$/.test(content)) // или начинается с \[ и заканчивается \]
            {
                code.outerHTML = code.innerHTML; // Удаляем <code></code>

                // Избегаем увеличения i, так как массив codes автоматически перестраивается,
                // и под индексом i будет уже следующий элемент
                continue;
            }
        }

        i++;
    }
}
