#!../bin/skycat_wish
#
# mkIndex.tcl - generate a tclIndex file in the current directory
# "@(#) $Id: mkIndex.tcl,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $"

package require Itcl
auto_mkindex . *.tcl
exit 0
