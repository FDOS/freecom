;; Installable Command sample template
;;
;;	<description>
;;

%macro icmdAction 0
	pusha
	<<<<<<<< Action performed within MUX-AE-01 >>>>>>>>>>>>>
	popa
%endmacro

%macro icmdComment 0
	DB '<<<<<<<<<<<<<<<< Comment record >>>>>>>>>>>>>>>>>>'
%endmacro

%macro icmdStuff 0
	<<<<<<<<<<<<<< Stuff required by Action >>>>>>>>>>>>>>>>
%endmacro

;; include the ICMD template
%include 'icmd_inc.inc'
