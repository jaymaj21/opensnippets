package require Tk
package require struct::list 

wm withdraw .
set input_file "";
if {[llength $argv] > 0} {
    set input_file [lindex $argv 0];
} else {
    set input_file [tk_getOpenFile];
}

regsub -all {\\} $input_file "/" input_file
puts "Expanding macros in file $input_file";
set fp [open $input_file r];

set output_text "";

 proc permutations {list {prefix ""}} {
    if {![llength $list]} then {return [list $prefix]}
    set res [list]
    set n 0
    foreach e $list {
        eval [list lappend res]\
          [permutations [lreplace $list $n $n] [linsert $prefix end $e]]
        incr n
    }
    return $res
 }

 proc comb {m n} {
    set set [list]
    for {set i 0} {$i < $n} {incr i} {lappend set $i}
    return [combinations $set $m]
}
proc combinations {list size} {
    if {$size == 0} {
        return [list [list]]
    }
    set retval {}
    for {set i 0} {($i + $size) <= [llength $list]} {incr i} {
        set firstElement [lindex $list $i]
        set remainingElements [lrange $list [expr {$i + 1}] end]
        foreach subset [combinations $remainingElements [expr {$size - 1}]] {
            lappend retval [linsert $subset 0 $firstElement]
        }
    }
    return $retval
}
 

 proc comma_separate {lst {lparen ""} {rparen ""}} {
    set res "";

    foreach item $lst {
        if {$res != ""} {
            append res ","
        }
        append res "${lparen}${item}${rparen}"
    }
    return $res;
 }


 proc suffixes {lst {result {}}} {
    set len [llength $lst];
    for {set i 0} {$i < $len} {incr i} {
        lappend  result [lrange $lst $i end];
    }
    return $result;
 }

proc prefixes {lst {result {}}} {
    set len [llength $lst];
    for {set i 0} {$i < $len} {incr i} {
        lappend  result [lrange $lst 0 $i];
    }
    return $result;
 }
 

proc add_text {args} {
    global output_text;
    foreach item $args {
        append output_text $item;
    }
}
clipboard clear

proc defmacro {name arglist body} {
    puts "defined macro $name"
    set cmd "proc macroexpand_**_$name { "
    append cmd $arglist
    append cmd " } { subst -nobackslashes -nocommands {"
    append cmd $body
    append cmd "}}"
    uplevel #0 $cmd
}

proc def_p_macro {name arglist body} {
    puts "defined procedural macro $name" 
    set cmd "proc macroexpand_**_$name {"
    append cmd $arglist
    append cmd " } { set code {};\n"
    append cmd $body;
    append cmd "; return \$code;}"
    uplevel #0 $cmd
}

proc remove-newline {lst} {
  return [regsub -all {[\r\n]} $lst { }]
}

proc expand_macro {name args}  {
    puts "expanding macro $name with arguments $args"
    set cmd "macroexpand_**_$name "
    append cmd $args;
    set result [uplevel #0 $cmd];
    add_text "\n//begin_macro_expansion : $name [remove-newline $args] {{{\n"
    add_text "#pragma region macro expansion\n"
    add_text $result;
    add_text "\n#pragma endregion $name [remove-newline $args]"
    add_text "\n//end_macro_expansion }}}\n"
}

proc seq {from to} {
    if {$from >= $to} {
        for {set i $from} {$i <= $to} {incr i}    {lappend out $i}
    } else {
        for {set i $from} {$i >= $to} {incr i -1} {lappend out $i}
    }
    return $out
}

set state "start";
set code "";
while {![eof $fp]} {
  set line [gets $fp];
  set pristine_line $line;

  if [regexp  {^//begin_macro_expansion} $line] {
        set state "expansion"
     } elseif  [regexp  {^//end_macro_expansion} $line] {
        set temp [gets $fp]; # discard a blank
        set stripped [string trimright [string trimleft $temp]];

        if {[string length $stripped] > 0} {
            add_text $temp "\n";
        }
        set state "start"
     } elseif {$state == "start" || $state == "code"} {
       add_text $line "\n";
     }
   
  if [regexp {^MACROS_END} $line] {
     uplevel #0 $code;
     set code "";
     set state "start";
     }
 
  if {$state == "code"} {
      append code $line "\n";
     }

  if [regexp {^/\*MACROS_BEGIN} $line] {
     set code "";
     set state "code";
     }


}

set output_text [string range $output_text 0 end-1];
puts $output_text
clipboard append $output_text
close $fp;
 
set yesno [tk_messageBox -title "Confirm source file modification" -message "This will expand the macros in the source file" -type yesno]
if {$yesno == yes} {
 set fp [open $input_file w];

 puts -nonewline $fp $output_text;
 close $fp
 puts "*********Modified source file with macro expansion!********";
 }

exit

