.text
.global solve
solve:
    #Acomodamos los coeficientes de las ecuaciones |A1|B1|A2|B2| en registro XMM0
     
    movsd (%rdi),%xmm0 #Mueve 2 flotantes simple precision alineados
    movhps 12(%rdi),%xmm0 #Movemos a la parte alta de xmm0 los coeficientes A2 y B2
    
    #-------------------------------------------------#
    #Acomodamos los valores del punto en registro XMM3
    
    movddup (%rsi),%xmm3
    
    #---------------------------------------------------------#
    #Multiplicamos XMM0 * XMM3 para realizar la transformacion
    
    mulps %xmm3, %xmm0

    #---------------------------------------------------------#
    #Acomodamos los coeficientes C de las ecuaciones |C1|0|0|C2|
    
    movss 20(%rdi),%xmm4
    shufps $0x39,%xmm4,%xmm4
    movss 8(%rdi),%xmm5
    movss %xmm5,%xmm4
    
    #---------------------------------------------------------#
    #Obtenemos los resultados |A1*X+C1|B1*Y|A2*X|B2*Y+C2|
    addps %xmm4,%xmm0
    
    #---------------------------------------------------------#
    #Obtenemos el resultado final
    haddps %xmm0, %xmm0
    movsd %xmm0, (%rsi)     
    ret        
    
