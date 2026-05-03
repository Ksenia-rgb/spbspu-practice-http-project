#ifndef HELP_HPP
#define HELP_HPP
#include <string>

namespace http
{
  namespace repl
  {
    namespace help
    {
      const std::string MAIN_HELP_TEXT = R"(HTTP Client - Console Utility

Usage:
  Start program: ./httpclient [session_name]
  If session_name is given, work begins in that named session.
  Without argument, work begins in an anonymous session.

Available commands:

Session management:
  session               Show current session name ("Unknown" if anonymous)
  session-list          List all saved sessions
  session-name <name>   Name (or rename) the current session
  session-rm            Delete the current session
  session-switch <name> Switch to another session (creates if not exists).
                        Use "Unknown" to switch to a new anonymous session.

History management:
				                            Show request history (JSON format).
  history <mark> <N>                History by mark. Show N latest requests.
  history <name>		                History by name. Show request with the entered name.
  history <N>                       Show N latest requests. Default.

  mark [req_name] <mark>            Mark a request (single word). If req_name omitted,
                                    marks the most recent request.
  comment [req_name] "<comment>"    Add a comment to a request. If req_name omitted,
                                    comments the most recent request.

Request input:
  req [req_name]                  Start interactive request builder. If req_name given,
                                  preload from that existing request; fields can be edited.
  file <req_name> <path>          Load HTTP request from a file (raw HTTP format).
  edit <new_name> [existing_name] Edit a request in text editor (GNU nano by default).
                                  If existing_name omitted, start from empty template.

Saving responses:
  save [req_name] <path>  Save server response to a file. If req_name omitted,
                          saves the most recent request's response.
                          Appends to file if not empty.

General:
  help                Show short help
  helpDetails         Show this help
  exit                Exit the program

Supported HTTP methods: GET, HEAD, POST

Supported headers: Host, User-Agent, Accept, Content-Type, Content-Length,
                   Cookie, Authorization, Connection, Set-Cookie, Location

Session notes:
- History stores last 1000 requests per session (oldest dropped)
- Named sessions are persisted between runs
- Anonymous sessions lost on exit
)";

      const std::string REQ_HELP_TEXT = R"(Request Builder Help

You are in REQ mode. Enter fields line by line in strict order.
Empty fields are allowed (press Enter to skip).

Sequence:
  1. req name>     Request name (required, unique per session)
  2. req method>   HTTP method: GET, HEAD, POST
  3. req URL>      Full URL (e.g., example.com/api/users)
                   Host header is auto-derived; do not specify it manually.
  4. req headers>  Header=value pairs separated by space. Values must be in quotes.
                   Example: Content-Type="application/json" Accept="*/*"
  5. req body>     Request body (two modes):
                   json string    Just enter json string like {"data": "123"}
                   json file      Enter path to json file. For example /home/user/body.json

After entering body, you will see "req command>". Supported commands:

  execute            Send the HTTP request and display response (JSON format)
  save <file>        Save response to file (warning if request not yet executed)
  show               Show request data
  name <name>        Edit request name
  method <method>    Edit HTTP method
  url <url>          Edit URL
  headers <headers>  Edit headers
  body <body>        Edit body
  help               Show short help
  helpDetails        Show this help
  exit               Exit REQ mode (warning if request not executed)
)";

    }
  }
}
#endif
