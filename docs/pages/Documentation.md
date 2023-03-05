# Documentation

The documentation can be build in two ways:
- Doxygen + [Awesome.css](https://jothepro.github.io/doxygen-awesome-css/)
- Doxygen + [poxy](https://github.com/marzer/poxy)

The documentation pages are written with [markdown](https://www.doxygen.nl/manual/markdown.html) syntax.


## Doxygen

Doxygen is a documentation generation tool that is widely used in the software development industry. It allows developers to generate documentation in a variety of formats, including HTML, LaTeX, and RTF, from source code comments.

Doxygen extracts documentation from specially formatted comments within the source code, such as comments beginning with /// or /*! */, and organizes it into a structured format. The resulting documentation can include information about functions, classes, variables, and other code elements, as well as details about their usage and parameters.

For installation instructions, see [Doxygen Homepage](https://www.doxygen.nl/download.html).

## Awesome.css
Build the doxygen + Awesome.css target with
```bash
cmake --build . --target Doxygen
```

## poxy
Poxy is based on Python 3 and can be installed with pip
```bash
pip install poxy
```

Build the doxygen + poxy target with
```bash
cmake --build . --target poxy
```