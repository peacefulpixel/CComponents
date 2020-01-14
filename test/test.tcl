#!/usr/bin/tclsh8.6

# test.tcl
#
# This script search test executable files by ::path::test_dir{TEST_NAME}::path::test_suf pattern
# then optionally runs all of them or all selected. If test exit code will be not 0, script will inform you about it
# and will print output.
#
# Flags was descripted in "test.tcl -help"

namespace eval message {
    proc glob_exception { error_msg } {
        return [ expr { "$::conf::print_escape" ? "\033\[1;37m\[ DEBUG \]\033\[0;33m Procedure \"glob\" produces exception. Probably no such tests in $::path::test_dir. Log: $error_msg\033\[0m"
                                                : "\[ DEBUG \] Procedure \"glob\" produces exception. Probably no such tests in $::path::test_dir. Log: $error_msg" } ] }
    proc tests_detected { count } {
        return [ expr { "$::conf::print_escape" ? "\033\[1;37m\[ DEBUG \]\033\[0;33m $count Tests detected. Running\033\[0m" 
                                                : "\[ DEBUG \] $count Tests detected. Running" } ] }
    proc tests_finished { count } {
        return [ expr { "$::conf::print_escape" ? "\033\[1;37m\[ DEBUG \]\033\[0;33m $count Tests finished\033\[0m"
                                                : "\[ DEBUG \] $count Tests finished" } ] }
    proc test_failed { name } {
        return [ expr { "$::conf::print_escape" ? "\033\[1;31m\[FAILED!\]\033\[0;33m Test $name was failed. Cause:\n\033\[0m"
                                                : "\[FAILED!\] Test $name was failed. Cause:\n" } ] }
    proc test_success { name } {
        return [ expr { "$::conf::print_escape" ? "\033\[1;32m\[SUCCESS\]\033\[0;33m Test $name was successfuly finished\033\[0m"
                                                : "\[SUCCESS\] Test $name was successfuly finished" } ] }
}

namespace eval conf {
    variable print_debug  0
    variable print_escape 1
}

namespace eval path {
    variable test_dir "tests/"
    variable test_suf ".o"
}

namespace eval tests {
    array set list {}

    proc test { test_name } {
        variable list
        set test_src $list($test_name)

        set code [ catch { exec "./$test_src" } output ]
        if   {$code != 0} { puts "[ ::message::test_failed  $test_name ]          $output" }\
        else              { puts  [ ::message::test_success $test_name ] }
    }

    proc fill_list { test_dir } {
        array set list_rs {}

        cd $test_dir
        set tfile_list {}
        if { [ catch { set tfile_list [ glob "*" ] } output ] 
        &&   $::conf::print_debug } {
            puts [ ::message::glob_exception $output ]
            return
        }

        foreach tfile $tfile_list {

            if { [ file isdirectory $tfile ] } {

                set current_dir [ pwd ]
                array set list_rsr [ fill_list $tfile ]
                foreach { key val } [ array get list_rsr ] {
                    set list_rs("$tfile/$key") "$tfile/$val"
                }
                cd $current_dir

            } elseif { [ string match "*$::path::test_suf" $tfile ] } {
                set list_rs("[ string trimright $tfile $::path::test_suf ]") "$tfile"
            }
        }

        return [ array get list_rs ]
    }

    proc test_all {} {

        variable list
        array set list_rs [ fill_list $::path::test_dir ]
        array set list [ array get list_rs ]

        if { $::conf::print_debug } { 
            puts [ ::message::tests_detected [ array size list ] ]
        }

        foreach { key val } [ array get list ] { test $key }

        if { $::conf::print_debug } { 
            puts [ ::message::tests_finished [ array size list ] ]
        }
    }
}

namespace eval main {
    proc help {} {
        puts "
    -help   Shows this message
    -debug  Prints debug output while testing
    -winout Prints output without escape sequences
    -nout   Do not print output
        "
    }

    proc start { argc, argv } {
        foreach arg $argv {
            switch -- $arg {
                -help   { help; return }
                -debug  { set ::conf::print_debug  1 }
                -winout { set ::conf::print_escape 0 }
            }
        }

        ::tests::test_all
    }
}

::main::start $argc $argv