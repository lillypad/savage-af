(load "~/quicklisp/setup.lisp")
(ql:quickload "cffi" :silent t)

(defpackage :cffi-user
  (:use :common-lisp :cffi))

(in-package :cffi-user)

(define-foreign-library savage-af
  (:unix (:or "./build/libsavage-af.so"))
  (t (:default "savage-af")))

(use-foreign-library savage-af)

;; Send HTTP Data
(defcfun ("http_send" http-send) :int
  (domain :string)
  (port :int)
  (data :string))


;; Validate IP Address
(defcfun ("validate_ip" validate-ip) :int
  (ip :string))


;; Validate Port
(defcfun ("validate_port" validate-port) :int
  (port :int))

;; Validate Domain
(defcfun ("validate_domain" validate-domain) :int
  (domain :string))

(defun http-headers (headers)
  "Create HTTP headers."
  (let ((data ""))
    (loop for i in headers do
         (setq data (concatenate 'string (car i) ": " (cdr i) '(#\return) '(#\newline)))
         (return data))))

(defun http-data (method path headers payload)
  "Send HTTP Data."
  (let ((data ""))
    (when (find method '("GET" "POST" "DELETE" "PUT" "OPTIONS" "CONNECT" "HEAD") :test #'equal)
      (setq data (concatenate 'string data method " " path " " "HTTP/1.0" '(#\return) '(#\newline) (http-headers headers) payload)))))

(defvar payload "{\"test\":123}")

(http-send "example.com" 80 (http-data "GET" "/index.html" '(("Content-Type" . "application/json")) payload))
