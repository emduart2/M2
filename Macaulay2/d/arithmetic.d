--		Copyright 1994-2006,2010 by Daniel R. Grayson

export nothing := void();
export uchar := integerType "unsigned char";
export short := integerType "short";
export ushort := integerType "unsigned short";
export uint := integerType "unsigned int";
export long := integerType "long";
export ulong := integerType "unsigned long";
export float := arithmeticType "float";
export ! (x:bool) ::= Ccode(bool,"(!(",x,"))");
export (x:bool) | (y:bool) ::= Ccode(bool,"((",x,")|(",y,"))");
export (x:bool) & (y:bool) ::= Ccode(bool,"(",x," & ",y,")");
export (x:bool) ^^ (y:bool) ::= Ccode(bool,"(",x," ^ ",y,")");
export (x:int) | (y:int) ::= Ccode(int,"(",x," | ",y,")");
export (x:uint) | (y:uint) ::= Ccode(uint,"(",x," | ",y,")");
export (x:ushort) | (y:ushort) ::= Ccode(ushort,"(",x," | ",y,")");
export (x:int) & (y:int) ::= Ccode(int,"(",x," & ",y,")");
export (x:uint) & (y:int) ::= Ccode(uint,"(",x," & ",y,")");
export (x:int) & (y:uint) ::= Ccode(uint,"(",x," & ",y,")");
export (x:uint) & (y:uint) ::= Ccode(uint,"(",x," & ",y,")");
export (x:ushort) & (y:ushort) ::= Ccode(ushort,"(",x," & ",y,")");
export (x:uint) ^^ (y:uint) ::= Ccode(uint,"(",x," ^ ",y,")");
export (x:ushort) ^^ (y:ushort) ::= Ccode(ushort,"(",x," ^ ",y,")");
export xor(x:int, y:int) ::= Ccode(int,"(",x," ^ ",y,")");
export xor(x:uint, y:uint) ::= Ccode(uint,"(",x," ^ ",y,")");
export (x:char) & (y:int) ::= Ccode(char,"(",x," & ",y,")");
export (x:int) & (y:char) ::= Ccode(char,"(",x," & ",y,")");
export (x:char) & (y:char) ::= Ccode(char,"(",x," & ",y,")");
export ~ (x:int) ::= Ccode(int,"(~ ",x,")");
export (x:uint) >> (y:int) ::= Ccode(uint,"(",x," >> ",y,")");
export (x:ushort) >> (y:int) ::= Ccode(ushort,"(",x," >> ",y,")");
export (x:int) >> (y:int) ::= Ccode(int,"(",x," >> ",y,")");
export (x:uint) << (y:int) ::= Ccode(uint,"(",x," << ",y,")");
export (x:ushort) << (y:int) ::= Ccode(ushort,"(unsigned short)(",x," << ",y,")");
export (x:int) << (y:int) ::= Ccode(int,"(",x," << ",y,")");
export (x:int) <  (y:int) ::= Ccode(bool,"(",x," < " ,y,")");
export (x:int) <= (y:int) ::= Ccode(bool,"(",x," <= ",y,")");
export (x:int) >  (y:int) ::= Ccode(bool,"(",x," > " ,y,")");
export (x:int) >= (y:int) ::= Ccode(bool,"(",x," >= ",y,")");
export (x:ushort) <  (y:ushort) ::= Ccode(bool,"(",x," < " ,y,")");
export (x:ushort) <= (y:ushort) ::= Ccode(bool,"(",x," <= ",y,")");
export (x:ushort) >  (y:ushort) ::= Ccode(bool,"(",x," > " ,y,")");
export (x:ushort) >= (y:ushort) ::= Ccode(bool,"(",x," >= ",y,")");
export (x:uint) <  (y:uint) ::= Ccode(bool,"(",x," < " ,y,")");
export (x:uint) <= (y:uint) ::= Ccode(bool,"(",x," <= ",y,")");
export (x:uint) >  (y:uint) ::= Ccode(bool,"(",x," > " ,y,")");
export (x:uint) >= (y:uint) ::= Ccode(bool,"(",x," >= ",y,")");
export (x:char) <  (y:char) ::= Ccode(bool,"(",x," < " ,y,")");
export (x:char) <= (y:char) ::= Ccode(bool,"(",x," <= ",y,")");
export (x:char) >  (y:char) ::= Ccode(bool,"(",x," > " ,y,")");
export (x:char) >= (y:char) ::= Ccode(bool,"(",x," >= ",y,")");
export (x:ulong) <  (y:ulong) ::= Ccode(bool,"(",x," < " ,y,")");
export (x:ulong) <= (y:ulong) ::= Ccode(bool,"(",x," <= ",y,")");
export (x:ulong) >  (y:ulong) ::= Ccode(bool,"(",x," > " ,y,")");
export (x:ulong) >= (y:ulong) ::= Ccode(bool,"(",x," >= ",y,")");
export (x:long) <  (y:long) ::= Ccode(bool,"(",x," < " ,y,")");
export (x:long) <= (y:long) ::= Ccode(bool,"(",x," <= ",y,")");
export (x:long) >  (y:long) ::= Ccode(bool,"(",x," > " ,y,")");
export (x:long) >= (y:long) ::= Ccode(bool,"(",x," >= ",y,")");
export (x:double) <  (y:double) ::= Ccode(bool,"(",x," <  " ,y,")");
export (x:double) <= (y:double) ::= Ccode(bool,"(",x," <= ",y,")");
export (x:double) >  (y:double) ::= Ccode(bool,"(",x," >  " ,y,")");
export (x:double) >= (y:double) ::= Ccode(bool,"(",x," >= ",y,")");
export (x:int) + (y:int) ::= Ccode(int,"(",x," + ",y,")");
export (x:uint) + (y:uint) ::= Ccode(uint,"(",x," + ",y,")");
export (x:int) - (y:int) ::= Ccode(int,"(",x," - ",y,")");
export (x:uint) - (y:uint) ::= Ccode(uint,"(",x," - ",y,")");
export (x:int) * (y:int) ::= Ccode(int,"(",x," * ",y,")");
export (x:uint) * (y:uint) ::= Ccode(uint,"(",x," * ",y,")");
export (x:int) / (y:int) ::= Ccode(int,"(",x," / ",y,")");
export (x:uint) / (y:uint) ::= Ccode(int,"(",x," / ",y,")");
export (x:ushort) / (y:ushort) ::= Ccode(ushort,"(",x," / ",y,")");
export (x:int) % (y:int) ::= Ccode(int,"(",x," % ",y,")");
export (x:uint) % (y:int) ::= Ccode(uint,"(",x," % ",y,")");
export (x:int) % (y:uint) ::= Ccode(uint,"(",x," % ",y,")");
export (x:uint) % (y:uint) ::= Ccode(uint,"(",x," % ",y,")");
export (x:ushort) % (y:ushort) ::= Ccode(ushort,"(",x," % ",y,")");
export (x:double) + (y:double) ::= Ccode(double,"(",x," + ",y,")");
export (x:double) - (y:double) ::= Ccode(double,"(",x," - ",y,")");
export (x:double) * (y:double) ::= Ccode(double,"(",x," * ",y,")");
export (x:double) / (y:double) ::= Ccode(double,"(",x," / ",y,")");
export (x:double) ^ (y:double) ::= Ccode(double,"pow(",x,",",y,")");
export (x:float) + (y:float) ::= Ccode(float,"(",x," + ",y,")");
export (x:float) - (y:float) ::= Ccode(float,"(",x," - ",y,")");
export (x:float) * (y:float) ::= Ccode(float,"(",x," * ",y,")");
export (x:float) / (y:float) ::= Ccode(float,"(",x," / ",y,")");
export (x:char) + (y:int ) ::= Ccode(char,"(",x," + ",y,")");
export (x:short) + (y:int ) ::= Ccode(short,"(",x," + ",y,")");
export (x:long) + (y:int ) ::= Ccode(long,"(",x," + ",y,")");
export (x:long) + (y:long) ::= Ccode(long,"(",x," + ",y,")");
export (x:long) - (y:int ) ::= Ccode(long,"(",x," - ",y,")");
export (x:long) % (y:int ) ::= Ccode(long,"(",x," % ",y,")");
export (x:long) * (y:int ) ::= Ccode(long,"(",x," * ",y,")");
export (x:long) / (y:int ) ::= Ccode(long,"(",x," / ",y,")");
export (x:long) / (y:long ) ::= Ccode(long,"(",x," / ",y,")");
export (x:int) + (y:long ) ::= Ccode(long,"(",x," + ",y,")");
export (x:int) - (y:long ) ::= Ccode(long,"(",x," - ",y,")");
export (x:int) % (y:long ) ::= Ccode(long,"(",x," % ",y,")");
export (x:int) * (y:long ) ::= Ccode(long,"(",x," * ",y,")");
export (x:int) / (y:long ) ::= Ccode(long,"(",x," / ",y,")");
export (x:ulong) + (y:int ) ::= Ccode(ulong,"(",x," + ",y,")");
export (x:ulong) - (y:int ) ::= Ccode(ulong,"(",x," - ",y,")");
export (x:ulong) % (y:int ) ::= Ccode(ulong,"(",x," % ",y,")");
export (x:ulong) * (y:int ) ::= Ccode(ulong,"(",x," * ",y,")");
export (x:ulong) / (y:int ) ::= Ccode(ulong,"(",x," / ",y,")");
export (x:int) + (y:ulong ) ::= Ccode(ulong,"(",x," + ",y,")");
export (x:int) - (y:ulong ) ::= Ccode(ulong,"(",x," - ",y,")");
export (x:int) % (y:ulong ) ::= Ccode(ulong,"(",x," % ",y,")");
export (x:int) * (y:ulong ) ::= Ccode(ulong,"(",x," * ",y,")");
export (x:int) / (y:ulong ) ::= Ccode(ulong,"(",x," / ",y,")");
export (x:long) < (y:int ) ::= Ccode(bool,"(",x," < ",y,")");
export (x:long) <= (y:int ) ::= Ccode(bool,"(",x," <= ",y,")");
export (x:long) > (y:int ) ::= Ccode(bool,"(",x," > ",y,")");
export (x:long) >= (y:int ) ::= Ccode(bool,"(",x," >= ",y,")");
export (x:ulong) < (y:int ) ::= Ccode(bool,"(",x," < ",y,")");
export (x:ulong) <= (y:int ) ::= Ccode(bool,"(",x," <= ",y,")");
export (x:ulong) > (y:int ) ::= Ccode(bool,"(",x," > ",y,")");
export (x:ulong) >= (y:int ) ::= Ccode(bool,"(",x," >= ",y,")");
export (x:char) - (y:int ) ::= Ccode(char,"(",x," - ",y,")");
export (x:int) + (y:char) ::= Ccode(char,"(",x," + ",y,")");
export (x:int) + (y:short) ::= Ccode(short,"(",x," + ",y,")");
export (x:int) - (y:char) ::= Ccode(char,"(",x," - ",y,")");
export (x:char) - (y:char) ::= Ccode(int,"(",x," - ",y,")");
export (x:uchar) + (y:int ) ::= Ccode(uchar,"(unsigned char)(",x," + ",y,")");
export (x:int) + (y:uchar) ::= Ccode(uchar,"(unsigned char)(",x," + ",y,")");
export (x:uchar) - (y:int ) ::= Ccode(uchar,"(unsigned char)(",x," - ",y,")");
export (x:int) - (y:uchar) ::= Ccode(uchar,"(unsigned char)(",x," - ",y,")");
export (x:ushort) + (y:int) ::= Ccode(ushort,"(unsigned short)(",x," + ",y,")");
export (x:int) + (y:ushort) ::= Ccode(ushort,"(unsigned short)(",x," + ",y,")");
export (x:ushort) - (y:int) ::= Ccode(ushort,"(unsigned short)(",x," - ",y,")");
export (x:int) - (y:ushort) ::= Ccode(ushort,"(unsigned short)(",x," - ",y,")");
export - (x:int) ::= Ccode(int,"(- ",x,")");
export - (x:double) ::= Ccode(double,"(- ",x,")");
export - (x:float) ::= Ccode(float,"(- ",x,")");
export - (x:long) ::= Ccode(long,"(- ",x,")");
export - (x:short) ::= Ccode(short,"(- ",x,")");
export (x:int) + (y:double) ::= Ccode(double,"(",x," + ",y,")");
export (x:double) + (y:int) ::= Ccode(double,"(",x," + ",y,")");
export (x:int) - (y:double) ::= Ccode(double,"(",x," - ",y,")");
export (x:double) - (y:int) ::= Ccode(double,"(",x," - ",y,")");
export (x:int) * (y:double) ::= Ccode(double,"(",x," * ",y,")");
export (x:double) * (y:int) ::= Ccode(double,"(",x," * ",y,")");
export (x:long) * (y:double) ::= Ccode(double,"(",x," * ",y,")");
export (x:double) * (y:long) ::= Ccode(double,"(",x," * ",y,")");
export (x:int) / (y:double) ::= Ccode(double,"(",x," / ",y,")");
export (x:ulong) / (y:double) ::= Ccode(double,"(",x," / ",y,")");
export (x:double) / (y:int) ::= Ccode(double,"(",x," / ",y,")");

-- Local Variables:
-- compile-command: "echo \"make: Entering directory \\`$M2BUILDDIR/Macaulay2/d'\" && echo \"make: Entering directory \\`$M2BUILDDIR/Macaulay2/d'\" && make -C $M2BUILDDIR/Macaulay2/d arithmetic.o "
-- End: