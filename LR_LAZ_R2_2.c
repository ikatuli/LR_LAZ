
#include <gtk/gtk.h>
#include <goocanvas.h>
#include <stdio.h>

//Всякая глобальная фигня
//
GtkWidget *HboxConva,*vbox,*HboxConva1;
GtkWidget *window;
GtkWidget *Stack,*Swit;
GtkWidget *HboxBatton;

unsigned int width, height;
//
//

void closeApp(GtkWidget *window, gpointer data) //Завершаем программу
{
    gtk_main_quit();

}

static void put_pixel (GdkPixbuf *TMPpixbuf, int x, int y, guchar red, guchar green, guchar blue){
  int width, height, row, n_channels;
  guchar *pixels, *p;

  n_channels = gdk_pixbuf_get_n_channels (TMPpixbuf);

  g_assert (gdk_pixbuf_get_colorspace (TMPpixbuf) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (TMPpixbuf) == 8);
  g_assert (!gdk_pixbuf_get_has_alpha (TMPpixbuf));
  g_assert (n_channels == 3);

  width = gdk_pixbuf_get_width (TMPpixbuf);
  height = gdk_pixbuf_get_height (TMPpixbuf);

  g_assert (x >= 0 && x < width);
  g_assert (y >= 0 && y < height);

  row = gdk_pixbuf_get_rowstride (TMPpixbuf);
  pixels = gdk_pixbuf_get_pixels (TMPpixbuf);

  p = pixels + y * row + x * n_channels;
  p[0] = red;
  p[1] = green;
  p[2] = blue;
}

unsigned char * get_pixel (GdkPixbuf *TMPpixbuf, int x, int y){
  int width, height, row, n_channels;
  guchar *pixels, *p;

  n_channels = gdk_pixbuf_get_n_channels (TMPpixbuf);

  g_assert (gdk_pixbuf_get_colorspace (TMPpixbuf) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (TMPpixbuf) == 8);
  g_assert (!gdk_pixbuf_get_has_alpha (TMPpixbuf));
  g_assert (n_channels == 3);

  width = gdk_pixbuf_get_width (TMPpixbuf);
  height = gdk_pixbuf_get_height (TMPpixbuf);

  g_assert (x >= 0 && x < width);
  g_assert (y >= 0 && y < height);

  row = gdk_pixbuf_get_rowstride (TMPpixbuf);
  pixels = gdk_pixbuf_get_pixels (TMPpixbuf);

  p = pixels + y * row + x * n_channels;
  return p;
}


void ConvaCreate2(GdkPixbuf *tmp_pixbuf,GtkBox *tmp_Box){
	//gtk_widget_destroy (tmp_canva);	
	GtkWidget *tmp_canva = goo_canvas_new ();
	GooCanvasItem *tmp_root;
	unsigned int tmp_width=gdk_pixbuf_get_width (tmp_pixbuf),tmp_height=gdk_pixbuf_get_height(tmp_pixbuf);
	gtk_widget_set_size_request (tmp_canva, tmp_width, tmp_height);
	goo_canvas_set_bounds (GOO_CANVAS (tmp_canva), 0, 0, tmp_width, tmp_width);
	
	gtk_box_pack_start(GTK_BOX(tmp_Box),tmp_canva,FALSE,FALSE,5);
	
	tmp_root = goo_canvas_get_root_item (GOO_CANVAS (tmp_canva));
	GooCanvasItem *ConvaImage = goo_canvas_image_new (tmp_root,tmp_pixbuf,0.0,0.0,"width",(gdouble)tmp_width,"height",(gdouble)tmp_width,NULL);}

GdkPixbuf * increase(GdkPixbuf *pixbuf)
{
	GdkPixbuf *tmp_pixbuf=gdk_pixbuf_new(GDK_COLORSPACE_RGB,FALSE,8,gdk_pixbuf_get_width(pixbuf)*2,gdk_pixbuf_get_height(pixbuf)*2);
	int x,y;
	for (y=0;y<gdk_pixbuf_get_height(pixbuf);y++){
		for (x=0;x<gdk_pixbuf_get_width (pixbuf);x++){
			guchar *tmp=get_pixel(pixbuf,x,y);
			put_pixel(tmp_pixbuf,2*x,2*y,tmp[0],tmp[1],tmp[2]);
			put_pixel(tmp_pixbuf,2*x+1,2*y,tmp[0],tmp[1],tmp[2]);
			put_pixel(tmp_pixbuf,2*x,2*y+1,tmp[0],tmp[1],tmp[2]);
			put_pixel(tmp_pixbuf,2*x+1,2*y+1,tmp[0],tmp[1],tmp[2]);
		}
	}
	return tmp_pixbuf;
}

GdkPixbuf * decrease (GdkPixbuf *pixbuf)
{
	GdkPixbuf *tmp_pixbuf=gdk_pixbuf_new(GDK_COLORSPACE_RGB,FALSE,8,gdk_pixbuf_get_width(pixbuf)/2,gdk_pixbuf_get_height(pixbuf)/2);
	int x,y;
	for (y=0;y<gdk_pixbuf_get_height(tmp_pixbuf);y++){
		for (x=0;x<gdk_pixbuf_get_width (tmp_pixbuf);x++){
			guchar *tmp=get_pixel(pixbuf,x*2,y*2);
			put_pixel(tmp_pixbuf,x,y,tmp[0],tmp[1],tmp[2]);
		}
	}
	return tmp_pixbuf;
}

void ConvaCreate (const char *filename){ //Canva
	gtk_window_resize(GTK_WINDOW(window),(3*width)+10,(2*height)+10);//Меняем размер окна
    GdkPixbuf *pixbuf,*pixbuf2;
	
	gtk_widget_destroy (HboxConva);
	gtk_widget_destroy (HboxConva1);
	gtk_widget_destroy (Stack);
	gtk_widget_destroy (Swit);

	HboxConva=gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);//Бокс для конвы
	HboxConva1=gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);	
	Swit=gtk_stack_switcher_new ();
	Stack=gtk_stack_new ();
	gtk_stack_add_titled (GTK_STACK(Stack),HboxConva,"0","Увеличение");
	gtk_stack_add_titled (GTK_STACK(Stack),HboxConva1,"1","Уменьшение");
	gtk_stack_set_transition_type (GTK_STACK (Stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_switcher_set_stack (GTK_STACK_SWITCHER (Swit), GTK_STACK (Stack));
	gtk_box_pack_start(GTK_BOX(vbox),Stack,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(HboxBatton),Swit,FALSE,FALSE,5);

	GError **tmp= NULL;
	pixbuf=gdk_pixbuf_new_from_file(filename,tmp);
	gtk_window_set_icon (GTK_WINDOW(window),pixbuf);

	ConvaCreate2(pixbuf,GTK_BOX(HboxConva));

	pixbuf2=increase(pixbuf);
	ConvaCreate2(pixbuf2,GTK_BOX(HboxConva));

	pixbuf2=decrease(pixbuf2);
	ConvaCreate2(pixbuf2,GTK_BOX(HboxConva));
	
//
	ConvaCreate2(pixbuf,GTK_BOX(HboxConva1));


	pixbuf2=decrease(pixbuf);
	ConvaCreate2(pixbuf2,GTK_BOX(HboxConva1));


	pixbuf2=increase(pixbuf2);
	ConvaCreate2(pixbuf2,GTK_BOX(HboxConva1));

	gtk_widget_show_all (vbox);

}

void BmpInfo(const char *filename){
	FILE *Image;	
	Image=fopen(filename,"rb");
	g_assert (Image != NULL);
	fseek(Image,18,SEEK_SET);
	fread(&width,4,1,Image);
	fread(&height,4,1,Image);
	fclose(Image);

	g_print("width:%d height:%d",width, height);

	ConvaCreate (filename);
}


void button_clicked(GtkWidget *button, gpointer data) //Кнопка для вызова окна выбора файла.
{
	GtkWidget *dialog;
	GtkFileFilter *filter;	
	gint res;
	dialog = gtk_file_chooser_dialog_new ("Выбор картинки",
                                      NULL,
                                      GTK_FILE_CHOOSER_ACTION_OPEN,
                                      ("_Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("_Open"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL); //Инициализация диалоги

	filter = gtk_file_filter_new();//Создаём фильтр
	gtk_file_filter_set_name(GTK_FILE_FILTER(filter),"Только bmp");
	gtk_file_filter_add_mime_type (GTK_FILE_FILTER(filter),"image/bmp");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

	res = gtk_dialog_run (GTK_DIALOG (dialog)); //Запускаем
	if (res == GTK_RESPONSE_ACCEPT) { //Если выбор сделан
		const char *filename;//Выбраный файл
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
		filename = gtk_file_chooser_get_filename (chooser);//Путь к файлу
		g_print ("%s\n",filename);
		BmpInfo(filename);
	}
	gtk_widget_destroy (dialog);//Закрыть окноget ();
}


int main(int argc, char *argv[])
{
    GtkWidget *file_load;



    gtk_init(&argc, &argv);

//Стили
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "gtk.css", NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider),1000);

//Параметр окна
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Laba");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

//Кнопка 
	
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);//Главный бокс
	HboxBatton=gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);//Бокс для кнопок

//Stack

	file_load = gtk_button_new_with_label("Загрузить файл!");
	gtk_box_pack_start(GTK_BOX(HboxBatton),file_load,FALSE,FALSE,5);

	gtk_box_pack_start(GTK_BOX(vbox),HboxBatton,FALSE,FALSE,5);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(closeApp), NULL);
	g_signal_connect(G_OBJECT(file_load), "clicked", G_CALLBACK(button_clicked), "Button 1");

    
	gtk_widget_show_all(window);


	gtk_main();

    return 0;
}
