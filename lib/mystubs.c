#define HTTPSERVER_IMPL
#include "httpserver.h"

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/fail.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <stdio.h>

typedef void (*request_handler)(struct http_request_s*);

CAMLprim value caml_hello(value unit) {
    printf("Hello from C!\n");
    return Val_unit;
}

CAMLprim value caml_http_response_init(value unit) {
    return (value)http_response_init();
}

CAMLprim value caml_http_response_status(value v_resp, value v_status) {
    struct http_response_s* resp = (struct http_response_s*)v_resp;
    http_response_status(resp, Int_val(v_status));
    return Val_unit;
}

CAMLprim value caml_http_response_header(value v_resp, value v_key,
                                         value v_value) {
    struct http_response_s* resp = (struct http_response_s*)v_resp;
    http_response_header(resp, String_val(v_key), String_val(v_value));
    return Val_unit;
}

CAMLprim value caml_http_response_body(value v_resp, value v_body) {
    struct http_response_s* resp = (struct http_response_s*)v_resp;
    http_response_body(resp, String_val(v_body), caml_string_length(v_body));
    return Val_unit;
}

CAMLprim value caml_http_respond(value v_req, value v_resp) {
    struct http_request_s* req = (struct http_request_s*)v_req;
    struct http_response_s* resp = (struct http_response_s*)v_resp;
    http_respond(req, resp);
    return Val_unit;
}

CAMLprim value caml_http_request_method(value v_req) {
    struct http_request_s* req = (struct http_request_s*)v_req;
    struct http_string_s mth = http_request_method(req);
    printf("[C] %s\n", mth.buf);
    return caml_copy_string("GET THIS");
}

CAMLprim value caml_http_request_body(value v_req) {
    struct http_request_s* req = (struct http_request_s*)v_req;
    struct http_string_s body = http_request_body(req);
    printf("[C] len=%d %s\n", body.len, body.buf);
    return caml_copy_string(body.buf);
}

CAMLprim value caml_http_request_target(value v_req) {
    struct http_request_s* req = (struct http_request_s*)v_req;
    struct http_string_s body = http_request_target(req);
    printf("[C] target len=%d %s\n", body.len, body.buf);
    return caml_copy_string(body.buf);
}

CAMLprim value caml_http_request_header(value v_req, value v_key) {
    struct http_request_s* req = (struct http_request_s*)v_req;
    struct http_string_s ret = http_request_header(req, String_val(v_key));
    return caml_copy_string(ret.buf);
}

void handle_request(struct http_request_s* request) {
    struct http_response_s* response = http_response_init();
    http_response_status(response, 200);
    http_response_header(response, "Content-Type", "text/plain");
    value v_response_body =
        caml_callback(*caml_named_value("caml_handle_request"), (value)request);
    http_response_body(response, v_response_body,
                       caml_string_length(v_response_body));
    http_respond(request, response);
}

CAMLprim value caml_http_server_init(value v_port) {
    printf("YO!\n");
    struct http_server_s* server =
        http_server_init(Int_val(v_port), handle_request);
    http_server_listen(server);
}
