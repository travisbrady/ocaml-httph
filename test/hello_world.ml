let printf = Printf.printf

let hack () =
    "hello, world!"

let hack2 request =
    let mth = Httph.http_request_method request in
    match mth with
    | "GET" -> "GOT IT " ^ (Httph.http_request_target request)
    | _ -> "NOTAGET"

let () =
    let hr = Httph.http_response_init () in
    Httph.http_response_status hr 200;
    Httph.http_response_header hr "Content-Type" "text/plain";
    Httph.http_server_init hack2 9000
