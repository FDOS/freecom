%ifidni MODEL,s
%include "small.inc"
%elifidni MODEL,m
%include "medium.inc"
%elifidni MODEL,l
%include "large.inc"
%else
%error "Unsupported MODEL; assign 's', 'm' or 'l' to macro MODEL"
%endif
