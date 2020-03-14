type _http_request
type http_response
external http_response_init : unit -> http_response = "caml_http_response_init"
external http_response_status : http_response -> int -> unit = "caml_http_response_status"
external http_response_header : http_response -> string -> string -> unit = "caml_http_response_header"
external http_response_body : http_response -> string -> unit = "caml_http_response_body"
external http_respond : _http_request -> http_response -> unit = "caml_http_respond"
external http_request_method : _http_request -> string ="caml_http_request_method"
external http_request_body : _http_request -> string ="caml_http_request_body"
external http_request_target : _http_request -> string = "caml_http_request_target"
external http_request_header : _http_request -> string -> string = "caml_http_request_header"
external http_request_has_flag : _http_request -> bool = "caml_http_request_has_flag"

external _http_server_init : int -> unit = "caml_http_server_init"

type http_request = {
    _method : string;
    target : string;
    body : string
}

let http_server_init f port =
    let _ = Callback.register "caml_handle_request" f in
    _http_server_init port

module Request = struct
    type t = _http_request
    let method' t = http_request_method t
    let body t = http_request_body t
    let target t = http_request_target t
    let header t = http_request_header t
end

module Response = struct
    type t = http_response
    let status t status_code = http_response_status t status_code
    let header t key value = http_response_header t key value
    let body t body = http_response_body t body
end
