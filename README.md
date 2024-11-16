<!-- YUCK:
I want to simply use Markdown for the Saber header...
Like this:
# `saber` C++ Header Library of Utility CLasses

But Doxygen renders the backtick-enclosed `saber` as:
<tt>saber</tt>

"<tt>"?! What the?!
So to workaround this, I have to use bogus embedded HTML instead
-->
<h1><code>saber</code><br/>C++ Header Library of Utility Classes</h1>

# `saber`<br/>C++ Header Library of Utility Classes<br/>

#### External Dependencies (requiring manual install)
- For API documentation generation
  - Doxygen (https://www.doxygen.nl/download.html)
  - Grafviz (https://graphviz.org/download/)
  (Used by Doxygen to generate diagrams/charts/etc.)