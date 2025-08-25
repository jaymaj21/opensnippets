/*MACROS_BEGIN

defmacro macro1 {a b c} {
   //This is just a test. The arguments are $a $b and $c.
   int i$a = get_value("$a");
   int i$b = get_value("$b");
   int i$c = get_value("$c");
}

# Procedural macro - compute the expansion
def_p_macro macro2 {args} {
    foreach arg $args {
    append code "\nBEGIN($arg);";
    }
    foreach arg [lreverse $args] {
    append code "\nEND($arg);";
    }
}

def_p_macro macro3 {args} {
    foreach p [permutations $args] {
        append code "\ninstantiate(" [comma_separate $p {"} {"}] ");"
    }
}


def_p_macro macro4 {args} {
    foreach c [combinations $args 3] {
        foreach p [permutations $c] {
            append code "\nINSTANTIATE(" [comma_separate $p ] ");"
        }
    }
}

def_p_macro macro5 {args} {
    foreach p [prefixes $args] {
        append code "\ninstantiate<" [comma_separate $p] "> {};"
    }
}

def_p_macro macro6 {args} {
    foreach p [prefixes $args {{}} ] {
        append code "\ninstantiate<" [comma_separate $p] "> {};"
    }
}

expand_macro macro1 P Q R
expand_macro macro2 A B C 
expand_macro macro2 "first argument"  "1,2,3,4" {if (test("arg")) {
    foo();
    } 
    else {
    bar();
    }
} 
expand_macro macro3 A B C D
expand_macro macro4 A B C D
expand_macro macro5 double double double double 
expand_macro macro6 int int int 
MACROS_END*/

//begin_macro_expansion : macro1 P Q R {{{
#pragma region macro expansion

   //This is just a test. The arguments are P Q and R.
   int iP = get_value("P");
   int iQ = get_value("Q");
   int iR = get_value("R");

#pragma endregion macro1 P Q R
//end_macro_expansion }}}

//begin_macro_expansion : macro2 A B C {{{
#pragma region macro expansion

BEGIN(A);
BEGIN(B);
BEGIN(C);
END(C);
END(B);
END(A);
#pragma endregion macro2 A B C
//end_macro_expansion }}}

//begin_macro_expansion : macro2 {first argument} 1,2,3,4 {if (test("arg")) {     foo();     }      else {     bar();     } } {{{
#pragma region macro expansion

BEGIN(first argument);
BEGIN(1,2,3,4);
BEGIN(if (test("arg")) {
    foo();
    } 
    else {
    bar();
    }
);
END(if (test("arg")) {
    foo();
    } 
    else {
    bar();
    }
);
END(1,2,3,4);
END(first argument);
#pragma endregion macro2 {first argument} 1,2,3,4 {if (test("arg")) {     foo();     }      else {     bar();     } }
//end_macro_expansion }}}

//begin_macro_expansion : macro3 A B C D {{{
#pragma region macro expansion

instantiate("A","B","C","D");
instantiate("A","B","D","C");
instantiate("A","C","B","D");
instantiate("A","C","D","B");
instantiate("A","D","B","C");
instantiate("A","D","C","B");
instantiate("B","A","C","D");
instantiate("B","A","D","C");
instantiate("B","C","A","D");
instantiate("B","C","D","A");
instantiate("B","D","A","C");
instantiate("B","D","C","A");
instantiate("C","A","B","D");
instantiate("C","A","D","B");
instantiate("C","B","A","D");
instantiate("C","B","D","A");
instantiate("C","D","A","B");
instantiate("C","D","B","A");
instantiate("D","A","B","C");
instantiate("D","A","C","B");
instantiate("D","B","A","C");
instantiate("D","B","C","A");
instantiate("D","C","A","B");
instantiate("D","C","B","A");
#pragma endregion macro3 A B C D
//end_macro_expansion }}}

//begin_macro_expansion : macro4 A B C D {{{
#pragma region macro expansion

INSTANTIATE(A,B,C);
INSTANTIATE(A,C,B);
INSTANTIATE(B,A,C);
INSTANTIATE(B,C,A);
INSTANTIATE(C,A,B);
INSTANTIATE(C,B,A);
INSTANTIATE(A,B,D);
INSTANTIATE(A,D,B);
INSTANTIATE(B,A,D);
INSTANTIATE(B,D,A);
INSTANTIATE(D,A,B);
INSTANTIATE(D,B,A);
INSTANTIATE(A,C,D);
INSTANTIATE(A,D,C);
INSTANTIATE(C,A,D);
INSTANTIATE(C,D,A);
INSTANTIATE(D,A,C);
INSTANTIATE(D,C,A);
INSTANTIATE(B,C,D);
INSTANTIATE(B,D,C);
INSTANTIATE(C,B,D);
INSTANTIATE(C,D,B);
INSTANTIATE(D,B,C);
INSTANTIATE(D,C,B);
#pragma endregion macro4 A B C D
//end_macro_expansion }}}

//begin_macro_expansion : macro5 double double double double {{{
#pragma region macro expansion

instantiate<double> {};
instantiate<double,double> {};
instantiate<double,double,double> {};
instantiate<double,double,double,double> {};
#pragma endregion macro5 double double double double
//end_macro_expansion }}}

//begin_macro_expansion : macro6 int int int {{{
#pragma region macro expansion

instantiate<> {};
instantiate<int> {};
instantiate<int,int> {};
instantiate<int,int,int> {};
#pragma endregion macro6 int int int
//end_macro_expansion }}}

// The end
