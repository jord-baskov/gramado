:SetUp
@echo off  

    cd c:\sdk\bin

:Monting
echo -{ ...
echo -{ Creating application  
	
:Compiling	
echo -{ ...
echo -{ Compiling ...

    gcc  -c c:\gramado\tests\fclass\crt0.c  -I c:\gramado\include\libc -o crt0.o  
	gcc  -c c:\gramado\tests\fclass\main.c  -I c:\gramado\include\libc -o main.o  
 
    
	copy c:\gramado\lib\ctype.o    c:\sdk\bin\ctype.o
    copy c:\gramado\lib\stdio.o    c:\sdk\bin\stdio.o
    copy c:\gramado\lib\stdlib.o   c:\sdk\bin\stdlib.o 
    copy c:\gramado\lib\string.o   c:\sdk\bin\string.o

	
 
:Objects	
set myObjects=crt0.o ^
main.o ^
ctype.o ^
stdio.o ^
stdlib.o ^
string.o  




:Linking  
echo -{ ...
echo -{ Linking objects ... 
   ld -T c:\gramado\tests\fclass\link.ld -o FCLASS.BIN %myObjects% -Map  c:\gramado\tests\fclass\map.s
   
   rem Não deletar os objetos.
 
:Moving   
   move FCLASS.BIN c:\gramado\bin\FCLASS.BIN
   
:End   
echo End!
	pause