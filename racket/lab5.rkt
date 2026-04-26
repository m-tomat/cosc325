#lang racket

; mean, median, mode, min, max of a list

(define (mean lst)
  (/ (apply + lst) (length lst)))

(define (median lst)
  (let* ([sorted (sort lst <)]
         [n (length sorted)]
         [mid (quotient n 2)])
    (if (odd? n)
        (list-ref sorted mid)
        (/ (+ (list-ref sorted (- mid 1)) (list-ref sorted mid)) 2))))

(define (mode lst)
  (let loop ([rem lst] [counts '()])
    (if (null? rem)
        (car (car (sort counts (lambda (a b) (> (cdr a) (cdr b))))))
        (let ([elem (car rem)]
              [match (assoc (car rem) counts)])
          (if match
              (loop (cdr rem)
                    (map (lambda (p)
                           (if (equal? (car p) elem) (cons elem (+ (cdr p) 1)) p))
                         counts))
              (loop (cdr rem) (cons (cons elem 1) counts)))))))

(define (minimum lst) (apply min lst))
(define (maximum lst) (apply max lst))

; test lists
(define listA '(3 7 12 5 9 12 4 8 6 12 10 2 14 12 1 11 13 12 15 12 16))
(define listB '(25 18 30 22 25 17 19 25 21 20 18 24 23 25 26 27 18 29 28 25))

(define (print-stats name lst)
  (printf "~a:\n" name)
  (printf "  mean   = ~a\n" (exact->inexact (mean lst)))
  (printf "  median = ~a\n" (exact->inexact (median lst)))
  (printf "  mode   = ~a\n" (mode lst))
  (printf "  min    = ~a\n" (minimum lst))
  (printf "  max    = ~a\n" (maximum lst)))

(print-stats "listA" listA)
(print-stats "listB" listB)
