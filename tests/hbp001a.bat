echo on
echo Root batch, echo on
echo starting hbp001b
call hbp001b
echo root batch: I'm back, this should be echo'd

echo off
echo Root batch, echo off
echo starting hbp001b
call hbp001b
echo root batch: I'm back, this should NOT be echo'd

