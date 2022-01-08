// Head partials
// Style
const String styles = "<style>"
  "body{}"
  "nav a:not(:last-child) {margin-right: 0.5em;}"
  "footer {margin-top: 1em;}"
  "</style>";

// Body partials
const String header = "<header>"
  + String(HOSTNAME) +
  "</header>";

const String nav = "<nav>"
  "<a href='/'>Status</a>"
  "<a href='/update_form'>Firmware update</a>"
  "</nav>";
  
const String footer= "<footer>"
  "  <div>Air conditioner controller firmware v0.4</div>"
  "  <div>Maxime Moreillon</div>"
  "</footer>";

// Update form
String update_form = "<h2>Firmware update</h2>"
  "<form method='POST' action='/update' enctype='multipart/form-data'>"
  "<input type='file' name='update'>"
  "<input type='submit' value='Update'>"
  "</form>";

// Combinations
String head ="<head>"
  "<title>" + String(HOSTNAME) +"</title>"
  + styles +
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "</head>";

String pre_main = "<html>"
  + head + 
  "<body>"
  "<div class='wrapper'>"
  + header + nav +
  "<div class='main'>";
  
String post_main = "</div>"
  + footer +
  "</div>"
  "</body>"
  "</html>";

// Root
String root_main = "";

// Not found
String not_found = "<h2>Not found</h2>";
