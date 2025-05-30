Text:     a a a b a a d a a b a a a
Index:    1 2 3 4 5 6 7 8 9 10 11 12 13

Align at pos 1:
P:        a a b a a a
          | | | | | |  (T[1-6])
          a a a b a a      <---- mismatch at 3rd char (T[3] != P[3]), X

Align at pos 2:
P:          a a b a a a
            | | | | | |  (T[2-7])
            a a b a a d      <---- mismatch at 6th char (T[7] != P[6]), X

Align at pos 3:
P:            a a b a a a
              | | | | | |  (T[3-8])
              a b a a d a      <---- mismatch at 2nd char (T[4] != P[2]), X

Align at pos 4:
P:              a a b a a a
                | | | | | |  (T[4-9])
                b a a d a a      <---- mismatch at 1st char (T[4] != P[1]), X

Align at pos 5:
P:                a a b a a a
                  | | | | | |  (T[5-10])
                  a a d a a b      <---- mismatch at 3rd char (T[7] != P[3]), X

Align at pos 6:
P:                  a a b a a a
                    | | | | | |  (T[6-11])
                    a d a a b a      <---- mismatch at 2nd char (T[7] != P[2]), X

Align at pos 7:
P:                    a a b a a a
                      | | | | | |  (T[7-12])
                      d a a b a a      <---- mismatch at 1st char (T[7] != P[1]), X

Align at pos 8:
P:                      a a b a a a
                        | | | | | |  (T[8-13])
                        a a b a a a      <---- all match, pattern found

Summary (comparison markers):

1:  | | X
2:  | | | | | X
3:  | X
4:  X
5:  | | X
6:  | X
7:  X
8:  | | | | | | (all match)