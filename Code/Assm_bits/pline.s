.pline_assm:
   cmpw       cr6,r3,r4                                        
   add        r5,r3,r5                                            
   clrlwi     r7,r3,30                                             
   sub        r11,r4,r3                                             
   subfic     r12,r7,4                                            
   clrlwi     r9,r4,30                                           
   addi       r10,r9,1                                               
   sub        r0,r11,r12                                             
   sub        r0,r0,r9                                            
   ble+        cr6,start                                
   blr                                                           
start:
   srawi.     r9,r0,2                                            
   mcrf       cr6,cr0                                           
   bge        cr6,not_little                           
   addi       r12,r11,1                                            
   cmpwi      cr1,r12,0                                               
   mtctr      r12                                           
   subi       r12,r5,1                                                             
   ble        cr1,pline_end                           
;   extsb      r11,r6                                                 
do_little:
   stbu       r6,0x0001(r12)	;was r11                                              
   bdnz       do_little                                  
   blr                                                             
not_little:
   cmpwi      cr1,r12,0                                              
   mtctr      r12                              
   subi       r12,r5,1                                            
   ble        cr1,no_start                           
;   extsb      r11,r6                                            
do_start:
   stbu       r6,0x0001(r12)	;was r11                                          
   bdnz       do_start                             
   addi       r5,r12,1
no_start:                                              
   ble        cr6,no_mid                                  
   mtctr      r9                            
   slwi       r3,r6,8                                           
   add        r4,r6,r3                                          
   slwi       r7,r6,16                                         
   subi       r12,r5,4                                         
   slwi       r0,r6,24                                          
   add        r8,r4,r7                                          
   add        r11,r8,r0                                         
mid:
   stwu       r11,0x0004(r12)                                        
   bdnz       mid                         
   addi       r5,r12,4                                        
no_mid:
   cmpwi      cr1,r10,0                           
   mtctr      r10                        
;   extsb      r11,r6	                                   
   subi       r12,r5,1                                 
   ble        cr1,no_end                 
do_end:
   stbu       r6,0x0001(r12)	;was r11                              
   bdnz       do_end                  
no_end:                                    
pline_end:
	blr
	EXPORT .pline_assm
	