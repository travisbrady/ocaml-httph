let printf = Printf.printf

module R=Httph.Request

let hack2 request response =
    let mth = R.method' request in
    Httph.Response.status response 200;
    Httph.Response.header response "Content-Type" "text/plain";
    match mth with
    | "GET" -> 
            Httph.Response.body response ("Got " ^ (R.target request));
            ()
    | _ ->
            Httph.Response.body response "Not a GET";
            ()

let () =
    (*
    let hr = Httph.http_response_init () in
    Httph.http_response_status hr 200;
    Httph.http_response_header hr "Content-Type" "text/plain";
    *)
    Httph.http_server_init hack2 9000
