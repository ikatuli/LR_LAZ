
#include <gtk/gtk.h>
#include <goocanvas.h>
#include <stdio.h>
#include <stdlib.h>

//Всякая глобальная фигня
//
GtkWidget *canvas;
GooCanvasItem *root;

//
//

void closeApp(GtkWidget *window, gpointer data) //Завершаем программу
{
    gtk_main_quit();

}

void cr_tr(float x,float y){
	GooCanvasItem *tr;
	tr=goo_canvas_polyline_new (root,TRUE,4,x,y, x+3,y-5,x+7,y-10,x+3,y-7,"stroke-color","#00FF00","line-width", 5.0,"fill-color","#00FF00" ,NULL);
}

void fun_trava(){
	int i,x,y;
	for (i=0; i<5;i++){
		x=rand()%250;
		y=rand()%240 +350;
		cr_tr((float)x,(float)y);
	}
}
                                  
int main(int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *vbox,*HboxBatton,*HboxConva;
    GtkWidget *oblaka,*trava;



    gtk_init(&argc, &argv);

//Стили
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "gtk.css", NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider),1000);

//Параметр окна
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Laba");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 600);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(closeApp), NULL);

//Коробки
	
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);//Главный бокс
	HboxBatton=gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);//Бокс для кнопок
	HboxConva=gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);//Бокс для конвы

	gtk_box_pack_start(GTK_BOX(vbox),HboxBatton,FALSE,FALSE,5);
	gtk_box_pack_end(GTK_BOX(vbox),HboxConva,FALSE,FALSE,5);
	gtk_container_add(GTK_CONTAINER(window), vbox);

//Кнопка

	trava=gtk_button_new_with_label("Трава!");
	gtk_box_pack_start(GTK_BOX(vbox),trava,FALSE,FALSE,5);
	g_signal_connect(G_OBJECT(trava), "clicked", G_CALLBACK(fun_trava), NULL);
//Конва
	
	canvas = goo_canvas_new ();
	gtk_widget_set_size_request (canvas, 250, 500);
	goo_canvas_set_bounds (GOO_CANVAS (canvas), 0, 0, 250, 500);
	gtk_box_pack_start(GTK_BOX(HboxConva),canvas,FALSE,FALSE,5);

	root = goo_canvas_get_root_item (GOO_CANVAS (canvas));

//Сам рисунок
	GooCanvasItem *sky,*glade,*HomeMaster,*HomeWindow,*frame,*roof,*sun,*SunLine;

	sky=goo_canvas_rect_new(root,0,0,250,250,"fill-color", "#00BFFF","stroke-color","#00BFFF",NULL);
	glade=goo_canvas_rect_new(root,0,250,250,250,"fill-color", "#228B22","stroke-color","#228B22",NULL);
	HomeMaster=goo_canvas_rect_new(root,50,200,100,100,"fill-color", "#8B4513","stroke-color","#8B4513",NULL);
	HomeWindow=goo_canvas_rect_new(root,80,220,40,40,"fill-color", "#778899","stroke-color","#778899",NULL);
	frame=goo_canvas_path_new (root,"M 79 240 L 121 240 M 100 240 L 100 260","stroke-color","#8B4513",NULL);
	roof=goo_canvas_path_new (root,"M 50 200 L 100 150 L 150 200 Z","fill-color", "#DEB887","stroke-color","#8B4513",NULL);
	sun=goo_canvas_ellipse_new(root,250,0,50,50,"stroke-color","#FFFF00","line-width", 5.0,"fill-color", "#FFFF00",NULL);
	SunLine=goo_canvas_path_new (root,"M 190 10 L 100 10 M 200 40 L 110 80 M 220 65 L 180 160 M 240 70 L 240 180","stroke-color","#FFFF00","line-width", 5.0,NULL);
//				  
	gtk_widget_show_all(window);
	gtk_main();

    return 0;
}
