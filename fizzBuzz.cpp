#include <iostream>
#include <assert.h>
using namespace std;

//typedef int(*L)(int);

int inc(int x) {return x+1;}
//int to_integer(int (*proc)(L, int)) { return proc(inc, 0); }
int to_integer(auto proc) { return proc(inc)(0); }
string to_bool(auto proc) { return proc(1)(0) ? "true" : "false";}

char to_c(auto n) {
    return "0123456789BFiuz"[to_integer(n)] ;
}
int main() {
    auto ZERO     = [] (auto l) -> auto {return [l] (auto x) -> auto {return x;};};
    auto ONE      = [] (auto l) -> auto {return [l] (auto x) -> auto {return l(x);};};
    auto TWO      = [] (auto l) -> auto {return [l] (auto x) -> auto {return l(l(x));};};
    auto THREE    = [] (auto l) -> auto {return [l] (auto x) -> auto {return l(l(l(x)));};};
    auto FIVE     = [] (auto l) -> auto {return [l] (auto x) -> auto {return l(l(l(l(l(x)))));};};
    auto FIVETEEN = [] (auto l) -> auto {return [l] (auto x) -> auto {return l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(x)))))))))))))));};};
    auto HUNDRED  = [] (auto l) -> auto {return [l] (auto x) -> auto {return l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(l(x))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));};};

    auto TRUE  = [] (auto x) -> auto {return [x] (auto y) -> auto {return x;};};
    auto FALSE = [] (auto x) -> auto {return [x] (auto y) -> auto {return y;};};

    auto IF = [] (auto l) -> auto {return l;};

    auto IS_ZERO = [FALSE, TRUE] (auto n) -> auto {
        return n ([FALSE] (auto x) -> auto {return FALSE;})(TRUE);
    };


    //INCREMENT = -> n { -> p { -> x { p[n[p][x]] } } }
    //DECREMENT = -> n { -> f { -> x { n[-> g { -> h { h[g[f]] } }]
    //                              [-> y { x }][-> y { y }] } } }

    auto INC = [] (auto n) -> auto { return [n] (auto l) -> auto {
            return  [n,l](auto x) -> auto {return l(n(l)(x));} ;
        };
    };


    auto DEC = [] (auto n) -> auto { 
        return [n] (auto f) -> auto {
            return [n,f] (auto x) -> auto {
                return n
                    ([f] (auto g) -> auto { return [g,f] (auto h) -> auto { return h(g(f)); }; }) 
                    ([x] (auto y) -> auto {return x;})
                    ([] (auto y)-> auto {return y;});
            };
        };
    };

    auto ADD = [INC] (auto n) -> auto { 
        return [n, INC] (auto m) -> auto {
            return n(INC)(m);
        };
    };

    auto PRINT = [INC] (auto n) -> auto { 
        return [n, INC] (auto m) -> auto {
            cout << to_integer(m) << " ";
            return n(INC)(m);
        };
    };
    auto PRINTC = [INC] (auto n) -> auto { 
        return [n, INC] (auto m) -> auto {
            cout << to_c(m) << "";
            return n(INC)(m);
        };
    };


    auto SUB = [DEC] (auto n) -> auto { 
        return [n, DEC] (auto m) -> auto {
            return m(DEC)(n);
        };
    };

    auto MUL = [ADD, ZERO] (auto n) -> auto {
        return [n, ADD, ZERO] (auto m) -> auto {
            return n(ADD(m))(ZERO);
        };
    };

    auto IS_LEQ = [SUB, IS_ZERO] (auto n) -> auto {
        return [n, SUB, IS_ZERO] (auto m) -> auto {
            return IS_ZERO(SUB(n)(m));
        };
    };
 
    //Z = -> f { -> x { f[-> y { x[x][y] }] }
    //          [-> x { f[-> y { x[x][y] }] }] }

    auto ZCOMB = [] (auto f) -> auto {
        return [f] (auto x) -> auto {
            return f([x](auto y) -> auto { return x(x)(y); }); 
        } 
        ([f] (auto x) -> auto { return f([x](auto y) -> auto { return x(x)(y); }); });
    };

    //MOD =
  //Z[-> f { -> m { -> n {
    //IF[IS_LESS_OR_EQUAL[n][m]][
      //-> x {
        //f[SUBTRACT[m][n]][n][x]
      //}
    //][
      //m
    //]
  //} } }]
    auto MOD = ZCOMB([IS_LEQ, SUB, IF] (auto f) -> auto {
        return [f, IS_LEQ, SUB, IF] (auto m) -> auto {
            return [m, f, IF, IS_LEQ, SUB] (auto n) -> auto { 
                return IF
                       (IS_LEQ(n)(m))
                       ([f, SUB, m, n](auto x) -> auto {return f(SUB(m)(n))(n)(x);})
                       (m);
            };
        };
    });
    //----------------------------------------
    auto PAIR = [] (auto x) -> auto { 
        return [x] (auto y) -> auto {
            return [x, y] (auto f) -> auto {
                return f(x)(y);
            };
        };
    };

    auto LEFT  = [] (auto p) -> auto { return p([] (auto x) -> auto { return [x] (auto y) -> auto {return x;};}); };
    auto RIGHT = [] (auto p) -> auto { return p([] (auto x) -> auto { return [x] (auto y) -> auto {return y;};}); };

    auto EMPTY = PAIR(TRUE)(TRUE);

    auto UNSHIFT = [PAIR, FALSE] (auto l) -> auto {
        return [PAIR, FALSE, l] (auto x) -> auto {
            return PAIR(FALSE)(PAIR(x)(l));
        };
    };

    auto IS_EMPTY = LEFT;

    auto HEAD = [LEFT, RIGHT] (auto l) -> auto { return  LEFT(RIGHT(l)); };
    auto TAIL = [LEFT, RIGHT] (auto l) -> auto { return RIGHT(RIGHT(l)); };

    //RANGE =
  //Z[-> f {
    //-> m { -> n {
      //IF[IS_LESS_OR_EQUAL[m][n]][
        //-> x {
          //UNSHIFT[f[INCREMENT[m]][n]][m][x]
        //}
      //][
        //EMPTY
      //]
    //} }
  //}]

    auto RANGE = ZCOMB([IF, IS_LEQ, UNSHIFT, INC, EMPTY] (auto f) ->  auto {
        return [IF, IS_LEQ, UNSHIFT, INC, EMPTY, f] (auto m) ->  auto {
            return [IF, IS_LEQ, UNSHIFT, INC, EMPTY, f, m] (auto n) ->  auto {
                return IF(IS_LEQ(m)(n))(
                    [UNSHIFT, f, INC, m ,n] (auto x) -> auto { return UNSHIFT(f(INC(m))(n))(m)(x); }
                    ) (EMPTY);
            };
        };
    });

//FOLD =
  //Z[-> f {
    //-> l { -> x { -> g {
      //IF[IS_EMPTY[l]][
        //x
      //][
        //-> y {
          //g[f[REST[l]][x][g]][FIRST[l]][y]
        //}
      //]
    //} } }
  //}]


    auto FOLD = ZCOMB(
            [IF, IS_EMPTY, TAIL, HEAD] (auto f) -> auto {
                return [IF, IS_EMPTY, TAIL, HEAD, f] (auto l) -> auto {
                    return [IF, IS_EMPTY, TAIL, HEAD, f, l] (auto x) -> auto {
                        return [IF, IS_EMPTY, TAIL, HEAD, f, l, x] (auto g) -> auto {
                            return IF
                                (IS_EMPTY(l))
                                (x)
                                ([g, f, TAIL, x, HEAD, l] (auto y) -> auto {return g(f(TAIL(l))(x)(g))(HEAD(l))(y);});
                        };
                    };
                };
            }
            
            );

//MAP =
  //-> k { -> f {
    //FOLD[k][EMPTY][
      //-> l { -> x { UNSHIFT[l][f[x]] } }
    //]
  //} }

    auto MAP = [FOLD, EMPTY, UNSHIFT] (auto k) -> auto {
        return [FOLD, EMPTY, UNSHIFT, k] (auto f) -> auto {
            return FOLD
                   (k)
                   (EMPTY)
                   ([UNSHIFT, f] (auto l) -> auto {
                        return [UNSHIFT, f, l] (auto x) -> auto {
                            return UNSHIFT(l)(f(x));
                        };
                    }
                   );
                
        };
    };

    //----------------------------------------
    auto TEN = MUL(TWO)(FIVE);

    auto B   = TEN;
    auto F   = INC(B);
    auto I   = INC(F);
    auto U   = INC(I);
    auto Z   = INC(U);

    auto FIZZ     = UNSHIFT(UNSHIFT(UNSHIFT(UNSHIFT(EMPTY)(Z))(Z))(I))(F);
    auto BUZZ     = UNSHIFT(UNSHIFT(UNSHIFT(UNSHIFT(EMPTY)(Z))(Z))(U))(B);
    auto FIZZBUZZ = UNSHIFT(UNSHIFT(UNSHIFT(UNSHIFT(BUZZ)(Z))(Z))(I))(F);
    //----------------------------------------
    //DIV =
  //Z[-> f { -> m { -> n {
    //IF[IS_LESS_OR_EQUAL[n][m]][
      //-> x {
        //INCREMENT[f[SUBTRACT[m][n]][n]][x]
      //}
    //][
      //ZERO
    //]
  //} } }]

    auto DIV = ZCOMB([IF, IS_LEQ, INC, SUB, ZERO](auto f) -> auto {
            return [IF, IS_LEQ, INC, SUB, f, ZERO](auto m) -> auto {
                return [IF, IS_LEQ, m, INC, f, SUB, ZERO](auto n) -> auto {
                    return IF
                    (IS_LEQ(n)(m))
                    ([INC, f, SUB, m, n, ZERO](auto x) -> auto {return INC(f(SUB(m)(n))(n))(x);})
                    (ZERO);
                };
            };
        }
    ); 


//PUSH =
  //-> l {
    //-> x {
      //FOLD[l][UNSHIFT[EMPTY][x]][UNSHIFT]
    //}
  //} 
    auto PUSH = [FOLD, UNSHIFT, EMPTY](auto l) -> auto {
        return [FOLD, UNSHIFT, EMPTY, l] (auto x) -> auto {
            return FOLD(l)(UNSHIFT(EMPTY)(x))(UNSHIFT);
        };
    }; 

    //Z[-> f { -> n { PUSH[
    //IF[IS_LESS_OR_EQUAL[n][DECREMENT[TEN]]][
      //EMPTY
    //][
      //-> x {
        //f[DIV[n][TEN]][x]
      //}
    //]
  //][MOD[n][TEN]] } }]

    auto TO_DIGITS = ZCOMB([PUSH, IF, IS_LEQ, DEC, TEN, DIV, EMPTY, MOD](auto f) -> auto {
            return [PUSH, IF, IS_LEQ, DEC, TEN, DIV, EMPTY, MOD, f](auto n) -> auto {
                return PUSH(
                    IF(IS_LEQ(n)(DEC(TEN)))(
                        EMPTY
                    )(
                        [f, DIV, TEN, n](auto x) -> auto {return f(DIV(n)(TEN))(x);}
                    )
                ) (
                    MOD(n)(TEN)
                );
            };
        }
    );
    //----------------------------------------

    //auto v = IF (TRUE)(IF(TRUE)(6)(7))(5);
    //cout << v << endl;

    //for (int i = _(ONE); i < _(HUNDRED); i++)
    //{
        //auto v=
            //(
        //IF (i % _(FIVETEEN) == 0) 
        //( "fizz buzz")
        //( 
            //(IF (i % _(FIVE) == 0) 
            //( "buzz")
            //( 
                //(IF (i % _(THREE) == 0) 
                //( "fizz")
                //( to_string(i)))
            //))
        //));
        //cout << v << end;
    //}

    assert(to_integer(ZERO)     == 0);
    assert(to_integer(ONE)      == 1);
    assert(to_integer(TWO)      == 2);
    assert(to_integer(THREE)    == 3);
    assert(to_integer(FIVETEEN) == 15);
    assert(to_integer(HUNDRED)  == 100);

    assert(to_bool(TRUE)        == "true");
    assert(to_bool(FALSE)       == "false");

    assert(to_bool(IS_ZERO(ONE))  == "false");
    assert(to_bool(IS_ZERO(ZERO)) == "true");
    assert(IF(TRUE)(5)(4)         == 5);
    assert(IF(FALSE)(5)(4)        == 4);

    assert(to_integer(INC(FIVE))            == 6);
    assert(to_integer(DEC(FIVE))            == 4);
    assert(to_integer(ADD(FIVE)(THREE))     == 8);
    assert(to_integer(SUB(FIVE)(THREE))     == 2);
    assert(to_integer(MUL(FIVE)(THREE))     == 15);
    assert(to_bool(IS_LEQ(FIVE)(THREE))     == "false");
    assert(to_bool(IS_LEQ(THREE)(FIVE))     == "true");
    assert(to_integer(MOD(FIVE)(THREE))     == 2);
    assert(to_integer(MOD(FIVETEEN)(THREE)) == 0);

    assert(to_integer(LEFT(PAIR(FIVE)(THREE)))  == 5);
    assert(to_integer(RIGHT(PAIR(FIVE)(THREE))) == 3);

    auto list = UNSHIFT(UNSHIFT(UNSHIFT(EMPTY)(THREE))(TWO))(ONE);
    assert(to_integer(HEAD(list)) == 1);
    assert(to_integer(HEAD(TAIL(list))) == 2);
    assert(to_integer(HEAD(TAIL(TAIL(list)))) == 3);
    assert(to_bool(IS_EMPTY(list)) == "false");
    assert(to_bool(IS_EMPTY(EMPTY)) == "true");

    //auto to_array = [IS_EMPTY, HEAD, TAIL] (auto list) -> string {
        //string s = "[";
        //auto l = list;
        //while(to_bool(IS_EMPTY(l)) == "false") {
            //s += to_string(to_integer(HEAD(l)));
            //s += ", ";
            //l = TAIL(l);
        //}

        //s += "]";
        //return s;
    //};

    //cout << to_array(list) << endl;
    //
    auto PRINT_LIST = [FOLD, ZERO, PRINT] (auto l) -> auto {return to_integer(FOLD(l)(ZERO)(PRINT)); };
    auto PRINT_STR  = [FOLD, ZERO, PRINTC] (auto l) -> auto {return to_integer(FOLD(l)(ZERO)(PRINTC)); };

    assert(to_integer(HEAD(RANGE(ONE)(FIVE))) == 1);
    assert(to_integer(HEAD(TAIL(RANGE(ONE)(FIVE)))) == 2);
    assert(to_integer(HEAD(TAIL(TAIL(RANGE(ONE)(FIVE))))) == 3);
    assert(to_integer(HEAD(TAIL(TAIL(TAIL(RANGE(ONE)(FIVE)))))) == 4);

    assert(to_integer(FOLD(RANGE(ONE)(FIVE))(ZERO)(ADD)) == 15);
    assert(to_integer(FOLD(RANGE(ONE)(FIVE))(ONE)(MUL)) == 120);

    //to_integer(PRINT(FIVE));
    //to_integer(MAP(RANGE(ONE)(FIVE))(PRINT)); cout << endl;
    PRINT_LIST(MAP(RANGE(ONE)(FIVE))(INC)); cout << endl;

    cout << to_c(Z) <<endl;
    PRINT_STR(FIZZ); cout << endl;
    PRINT_STR(BUZZ); cout << endl;
    PRINT_STR(FIZZBUZZ); cout << endl;
    PRINT_STR(TO_DIGITS(HUNDRED)); cout << endl;

}
