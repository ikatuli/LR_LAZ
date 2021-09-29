PREFIX =./
GOOCANVAS = `pkg-config --cflags --libs goocanvas-3.0`
GTK3 = `pkg-config --cflags --libs gtk+-3.0`

.PHONY: all clean
	
all: LR_LAZ_R1 LR_LAZ_R2_1 LR_LAZ_R2_2 LR_LAZ_R2_3 LR_LAZ_R3 LR_LAZ_V

clean:
	rm -rf $(PREFIX)/LR_LAZ_R1 $(PREFIX)/LR_LAZ_R2_1 $(PREFIX)/LR_LAZ_R2_2 $(PREFIX)/LR_LAZ_R2_3 $(PREFIX)/LR_LAZ_R2_3 $(PREFIX)/LR_LAZ_R3 $(PREFIX)/LR_LAZ_V 

LR_LAZ_R1: LR_LAZ_R1.c
	gcc -g LR_LAZ_R1.c -o $(PREFIX)/LR_LAZ_R1 $(GTK3)  $(GOOCANVAS)

LR_LAZ_R2_1:LR_LAZ_R2_1.c
	gcc -g LR_LAZ_R2_1.c -o $(PREFIX)/LR_LAZ_R2_1 $(GTK3) $(GOOCANVAS)
LR_LAZ_R2_2:LR_LAZ_R2_2.c
	gcc -g LR_LAZ_R2_2.c -o $(PREFIX)/LR_LAZ_R2_2 $(GTK3) $(GOOCANVAS)
LR_LAZ_R2_3:LR_LAZ_R2_3.c
	gcc -g LR_LAZ_R2_3.c -o $(PREFIX)/LR_LAZ_R2_3 $(GTK3) $(GOOCANVAS)
LR_LAZ_R3: LR_LAZ_R3.c
	gcc -g LR_LAZ_R3.c -o $(PREFIX)/LR_LAZ_R3 $(GTK3) `pkg-config --cflags --libs gdk-pixbuf-2.0` `pkg-config --cflags --libs plplot`

LR_LAZ_V: LR_LAZ_V.c
	gcc -g LR_LAZ_V.c -o $(PREFIX)/LR_LAZ_V $(GTK3) $(GOOCANVAS)

