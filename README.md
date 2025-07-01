\# FresnoF24 Interpreter



A recursive-descent interpreter built in C++ for \*\*FresnoF24\*\*, a mini-language supporting typed variable declarations, assignments, arithmetic expressions, and print statements.



\## 📜 Language Features



FresnoF24 programs follow this structure:



```fresnof24

program

&nbsp; int a, b;

&nbsp; double d;

begin

&nbsp; a = 2 \* (5 + 7);

&nbsp; b = a;

&nbsp; print a;

&nbsp; print b;

end

```



\### ✅ Supported Features



\- Variable declarations (`int`, `double`)

\- Arithmetic expressions with precedence: `+`, `-`, `\*`, `/`, `^`

\- Parentheses and multi-digit numbers

\- Variable assignments and print statements

\- Input via file, output to console



\## 🧠 Interpreter Highlights



\- Implemented via recursive-descent parsing

\- Evaluates expressions during parsing (no AST required)

\- Tracks symbol table for type checking and variable values

\- Detects and reports:

&nbsp; - Lexical errors

&nbsp; - Syntax errors

&nbsp; - Semantic errors



\## 🗂️ Files



\- `Csci\_117\_Lab4.cpp` – Main interpreter implementation

\- `input.txt` – Sample FresnoF24 input program

\- `README.md` – You’re reading it

\- `.gitignore` – Keeps the repo clean (ignores `.vs/`, `x64/`, etc.)



\## 🚀 How to Use



1\. Compile with g++ or your preferred compiler:

&nbsp;  ```bash

&nbsp;  g++ -o fresnof24 Csci\_117\_Lab4.cpp

&nbsp;  ```

2\. Ensure `input.txt` contains your FresnoF24 program.

3\. Run the interpreter:

&nbsp;  ```bash

&nbsp;  ./fresnof24

&nbsp;  ```

4\. View output in the console.



\## 📌 Example Output



Given this input:



```fresnof24

int a;

begin

&nbsp; a = (2 + 3) \* 4;

&nbsp; print a;

end

```



Expected output:

```

20

```



\## 📝 License



MIT License – see `LICENSE` file for details.

