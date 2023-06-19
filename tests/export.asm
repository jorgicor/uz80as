	.EXPORT 
	.EXPORT label1,
	.EXPORT label3
	.EXPORT label2, label6789012345678

	.ORG $4000

label1	LD A,B
label2	LD B,C
label3  LD A,B
label6789012345678 LD B,C

	.END
