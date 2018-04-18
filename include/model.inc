%ifidni MODEL,s
%include "../include/small.inc"
%elifidni MODEL,m
%include "../include/medium.inc"
%elifidni MODEL,l
%include "../include/large.inc"
%else
%error "Unsupported MODEL; assign 's', 'm' or 'l' to macro MODEL"
%endif
