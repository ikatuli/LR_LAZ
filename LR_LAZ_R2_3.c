
#include <gtk/gtk.h>
#include <goocanvas.h>
#include <stdio.h>

//Всякая глобальная фигня
//
GtkWidget *HboxConva,*HboxRBatton,*HboxRBatton2;
GtkWidget *window;
GtkWidget *canvas,*canvas2;
GdkPixbuf *pixbuf,*pixbuf2;
GooCanvasItem *root2;
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



void BmpBuf(const char *filename,unsigned int width, unsigned int height){//Читаем цвета bmp
	FILE *Image;
	unsigned int Imsize;
	Image=fopen(filename,"rb");

	fseek(Image,10,SEEK_SET); //Смещение изображения
	fread(&Imsize,4,1,Image);


	unsigned int bit24;
	fseek(Image,28,SEEK_SET); //Смещение изображения
	fread(&bit24,2,1,Image);

	g_assert (bit24=24);

	fseek(Image,Imsize,SEEK_SET);//Установка на данные пикселей
	guchar * IndexPixel;
	pixbuf=gdk_pixbuf_new(GDK_COLORSPACE_RGB,FALSE,8,width,height);//Создаём картинку в памяти
	IndexPixel=gdk_pixbuf_get_pixels(pixbuf);//Для обращения к пикселям.

	int x,y,tmp;
	guchar red, green, blue;
	for (y=(gdk_pixbuf_get_height(pixbuf)-1);y>=0;y--){
		for (x=0;x<gdk_pixbuf_get_width (pixbuf);x++){
			fread(&blue,1,1,Image);
			fread(&green,1,1,Image);
			fread(&red,1,1,Image);
			put_pixel(pixbuf,x,y,red,green,blue);

			if (x==(gdk_pixbuf_get_width (pixbuf)-1)) { //Читаем лишние байты
				for (tmp=0;tmp<(gdk_pixbuf_get_width (pixbuf)%4);tmp++){
				fread(&blue,1,1,Image);
				}
			}
		}
	}
	fclose(Image);

}

void ConvaCreate (const char *filename){ //Canva
	gtk_window_resize(GTK_WINDOW(window),(2*width),height+10);//Меняем размер окна
	gtk_widget_destroy (canvas);
	gtk_widget_destroy (canvas2);
	GooCanvasItem *root;


	canvas = goo_canvas_new ();
	gtk_widget_set_size_request (canvas, width, height);
	goo_canvas_set_bounds (GOO_CANVAS (canvas), 0, 0, width+5, height+5);
	gtk_box_pack_start(GTK_BOX(HboxConva),canvas,FALSE,FALSE,5);

	root = goo_canvas_get_root_item (GOO_CANVAS (canvas));
	BmpBuf(filename,width,height);
	//GError **tmp= NULL;
	//pixbuf=gdk_pixbuf_new_from_file(filename,tmp);
	gtk_window_set_icon (GTK_WINDOW(window),pixbuf);
	GooCanvasItem *ConvaImage = goo_canvas_image_new (root,pixbuf,0.0,0.0,"width",(gdouble)width,"height",(gdouble)height,NULL);


	gtk_widget_show_all (HboxConva);
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
		const char *filename;//Выбранный файл
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
		filename = gtk_file_chooser_get_filename (chooser);//Путь к файлу
		g_print ("%s\n",filename);
		gtk_widget_show (HboxRBatton);
		gtk_widget_show (HboxRBatton2);
		BmpInfo(filename);
	}
	gtk_widget_destroy (dialog);//Закрыть окно get ();
}
void ConvaCreate2(){
	gtk_widget_destroy (canvas2);	
	canvas2 = goo_canvas_new ();
	gtk_widget_set_size_request (canvas2, width, height);
	goo_canvas_set_bounds (GOO_CANVAS (canvas2), 0, 0, width+5, height+5);
	gtk_box_pack_start(GTK_BOX(HboxConva),canvas2,FALSE,FALSE,5);

	root2 = goo_canvas_get_root_item (GOO_CANVAS (canvas2));
	GooCanvasItem *ConvaImage2 = goo_canvas_image_new (root2,pixbuf2,0.0,0.0,"width",(gdouble)width,"height",(gdouble)height,NULL);

}

void clean_conva(){
	ConvaCreate2();
}


void fun_vref (){
	ConvaCreate2();
	pixbuf2= gdk_pixbuf_copy (pixbuf);
	guchar * IndexPixel2=gdk_pixbuf_get_pixels(pixbuf2);
	guchar * IndexPixel=gdk_pixbuf_get_pixels(pixbuf);
	int row=gdk_pixbuf_get_rowstride (pixbuf),x,y;
	for (y=0;y<gdk_pixbuf_get_height(pixbuf);y++){
		for (x=0;x<gdk_pixbuf_get_width (pixbuf);x++){
		guchar *tmp=get_pixel(pixbuf,x,y);
		guchar S=0.5*tmp[0]+0.5*tmp[1]+0.5*tmp[2];
		put_pixel(pixbuf2,x,y,S,S,S);
		}
	}
	GooCanvasItem *ConvaImage2 = goo_canvas_image_new (root2,pixbuf2,0.0,0.0,NULL);

	gtk_widget_show_all (canvas2);
}

void fun_href (){
	ConvaCreate2();
	pixbuf2= gdk_pixbuf_copy (pixbuf);
	guchar * IndexPixel2=gdk_pixbuf_get_pixels(pixbuf2);
	guchar * IndexPixel=gdk_pixbuf_get_pixels(pixbuf);
	int row=gdk_pixbuf_get_rowstride (pixbuf),x,y;
	for (y=0;y<gdk_pixbuf_get_height(pixbuf);y++){
		for (x=0;x<gdk_pixbuf_get_width (pixbuf);x++){
		guchar *tmp=get_pixel(pixbuf,x,y);
		guchar S=0.333*tmp[0]+0.333*tmp[1]+0.333*tmp[2];
		put_pixel(pixbuf2,x,y,S,S,S);
		}
	}
	GooCanvasItem *ConvaImage2 = goo_canvas_image_new (root2,pixbuf2,0.0,0.0,NULL);

	gtk_widget_show_all (canvas2);
}

void fun_lhalf (){
	ConvaCreate2();
	pixbuf2= gdk_pixbuf_copy (pixbuf);
	guchar * IndexPixel2=gdk_pixbuf_get_pixels(pixbuf2);
	guchar * IndexPixel=gdk_pixbuf_get_pixels(pixbuf);
	int row=gdk_pixbuf_get_rowstride (pixbuf),x,y;
	for (y=0;y<gdk_pixbuf_get_height(pixbuf);y++){
		for (x=0;x<gdk_pixbuf_get_width (pixbuf);x++){
		guchar *tmp=get_pixel(pixbuf,x,y);
		guchar S=0.299*tmp[0]+0.587*tmp[1]+0.114*tmp[2];
		put_pixel(pixbuf2,x,y,S,S,S);
		}
	}
	GooCanvasItem *ConvaImage2 = goo_canvas_image_new (root2,pixbuf2,0.0,0.0,NULL);

	gtk_widget_show_all (canvas2);
}
void fun_rhalf (){
	ConvaCreate2();
	pixbuf2= gdk_pixbuf_copy (pixbuf);
	guchar * IndexPixel2=gdk_pixbuf_get_pixels(pixbuf2);
	guchar * IndexPixel=gdk_pixbuf_get_pixels(pixbuf);
	int row=gdk_pixbuf_get_rowstride (pixbuf),x,y;
	for (y=0;y<gdk_pixbuf_get_height(pixbuf);y++){
		for (x=0;x<gdk_pixbuf_get_width (pixbuf);x++){
		guchar *tmp=get_pixel(pixbuf,x,y);
		guchar S=0.2126*tmp[0]+0.7152*tmp[1]+0.0722*tmp[2];
		put_pixel(pixbuf2,x,y,S,S,S);
		}
	}
	GooCanvasItem *ConvaImage2 = goo_canvas_image_new (root2,pixbuf2,0.0,0.0,NULL);

	gtk_widget_show_all (canvas2);
}
void fun_uhalf (){
	ConvaCreate2();
	pixbuf2= gdk_pixbuf_copy (pixbuf);
	guchar * IndexPixel2=gdk_pixbuf_get_pixels(pixbuf2);
	guchar * IndexPixel=gdk_pixbuf_get_pixels(pixbuf);
	int row=gdk_pixbuf_get_rowstride (pixbuf),x,y;
	for (y=0;y<gdk_pixbuf_get_height(pixbuf);y++){
		for (x=0;x<gdk_pixbuf_get_width (pixbuf);x++){
		guchar *tmp=get_pixel(pixbuf,x,y);
		guchar S=0.59*tmp[0]+0.11*tmp[1]+0.3*tmp[2];
		put_pixel(pixbuf2,x,y,S,S,S);
		}
	}
	GooCanvasItem *ConvaImage2 = goo_canvas_image_new (root2,pixbuf2,0.0,0.0,NULL);

	gtk_widget_show_all (canvas2);
}
void fun_bhalf (){
	ConvaCreate2();
	pixbuf2= gdk_pixbuf_copy (pixbuf);
	guchar * IndexPixel2=gdk_pixbuf_get_pixels(pixbuf2);
	guchar * IndexPixel=gdk_pixbuf_get_pixels(pixbuf);
	int row=gdk_pixbuf_get_rowstride (pixbuf),x,y;
	for (y=0;y<gdk_pixbuf_get_height(pixbuf);y++){
		for (x=0;x<gdk_pixbuf_get_width (pixbuf);x++){
		guchar *tmp=get_pixel(pixbuf,x,y);
		guchar S=0.4*tmp[0]+0.4*tmp[1]+0.2*tmp[2];
		put_pixel(pixbuf2,x,y,S,S,S);
		}
	}
	GooCanvasItem *ConvaImage2 = goo_canvas_image_new (root2,pixbuf2,0.0,0.0,NULL);

	gtk_widget_show_all (canvas2);
}

int main(int argc, char *argv[])
{
	GtkWidget *vbox,*HboxBatton,*VboxBatton;
    GtkWidget *file_load;
	GtkWidget *Copy,*Vref,*Href,*Lhalf,*Rhalf,*Uhalf,*Bhalf,*CleanConva;


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
	VboxBatton=gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);//Бокс для кнопок	
	HboxRBatton = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	HboxRBatton2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);


	
	HboxConva=gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);//Бокс для конвы

	file_load = gtk_button_new_with_label("Загрузить файл!");
	gtk_box_pack_start(GTK_BOX(HboxBatton),file_load,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(HboxBatton),VboxBatton,FALSE,FALSE,5);

	gtk_box_pack_start(GTK_BOX(vbox),HboxBatton,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(vbox),HboxConva,FALSE,FALSE,5);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(closeApp), NULL);
	g_signal_connect(G_OBJECT(file_load), "clicked", G_CALLBACK(button_clicked), "Button 1");

	//Радио кнопки
	//gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(Copy));
	//


	 CleanConva=gtk_radio_button_new_with_label  (NULL,"Очистка");
	 Vref =gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON(CleanConva), "0.5");
	 Href =gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON(CleanConva), "0.333");
	 Lhalf=gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON(CleanConva), "PAL");
	 Rhalf=gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON(CleanConva), "HDTV");
	 Uhalf=gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON(CleanConva), "Test");
	 Bhalf=gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON(CleanConva), "Channel Mixer");

	 g_signal_connect(G_OBJECT(Vref), "toggled", G_CALLBACK(fun_vref), NULL);
	 g_signal_connect(G_OBJECT(Href), "toggled", G_CALLBACK(fun_href), NULL);
	 g_signal_connect(G_OBJECT(Lhalf), "toggled", G_CALLBACK(fun_lhalf), NULL);
	 g_signal_connect(G_OBJECT(Rhalf), "toggled", G_CALLBACK(fun_rhalf), NULL);
	 g_signal_connect(G_OBJECT(Uhalf), "toggled", G_CALLBACK(fun_uhalf), NULL);
	 g_signal_connect(G_OBJECT(Bhalf), "toggled", G_CALLBACK(fun_bhalf), NULL);
	 g_signal_connect(G_OBJECT(CleanConva), "toggled", G_CALLBACK(clean_conva), NULL);


	 gtk_box_pack_start(GTK_BOX(HboxRBatton),CleanConva,FALSE,FALSE,5);
	 gtk_box_pack_start(GTK_BOX(HboxRBatton),Vref,FALSE,FALSE,10);
	 gtk_box_pack_start(GTK_BOX(HboxRBatton),Href,FALSE,FALSE,10);
	 gtk_box_pack_start(GTK_BOX(HboxRBatton2),Lhalf,FALSE,FALSE,5);
	 gtk_box_pack_start(GTK_BOX(HboxRBatton2),Rhalf,FALSE,FALSE,5);
	 gtk_box_pack_start(GTK_BOX(HboxRBatton2),Uhalf,FALSE,FALSE,5);
	 gtk_box_pack_start(GTK_BOX(HboxRBatton2),Bhalf,FALSE,FALSE,5);

	 gtk_box_pack_start(GTK_BOX(VboxBatton),HboxRBatton,FALSE,FALSE,5);
	 gtk_box_pack_start(GTK_BOX(VboxBatton),HboxRBatton2,FALSE,FALSE,5);
	//

    
	gtk_widget_show_all(window);
	gtk_widget_hide (HboxRBatton);
	gtk_widget_hide (HboxRBatton2);

	gtk_main();

    return 0;
}
