%ifidni MODEL,s
%include "../include/small.inc"
%elifidni MODEL,c
%include "../include/compact.inc"
%elifidni MODEL,m
%include "../include/medium.inc"
%elifidni MODEL,l
%include "../include/large.inc"
%elifidni MODEL,h
%include "../include/huge.inc"
%else
%error "Unsupported MODEL; assign 's', 'c', 'm', or 'l' to macro MODEL"
%endif
