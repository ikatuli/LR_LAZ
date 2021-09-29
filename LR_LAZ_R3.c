#include <gtk/gtk.h>
#include "plplot.h"
#include <gdk-pixbuf/gdk-pixbuf.h>

//Всякая глобальная фигня
//
int znach[256];
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

int grey (GdkPixbuf *tmp_pix){
	guchar * IndexPixel=gdk_pixbuf_get_pixels(tmp_pix);
	int row=gdk_pixbuf_get_rowstride (tmp_pix),x,y,max=0;
	for (y=0;y<gdk_pixbuf_get_height(tmp_pix);y++){
		for (x=0;x<gdk_pixbuf_get_width (tmp_pix);x++){
		guchar *tmp=get_pixel(tmp_pix,x,y);
		int ind=tmp[0];
		znach[ind]++;
		if (znach[ind]>max){max=znach[ind];}
		//g_print("%u:%u \n",ind,znach[ind]);
		}
	}
	return max;
}

void plplot_05(int max)
{
	plsdev ("xcairo");
	plinit();
	plcol0(15);
	plenv(0.0,255.0,1,max+100,0,0);
	plcol0( 3 );
	pllab( "#frУровень серого в изображении", "#frКоличество пикселей","#frКоличество значений для каждого уровня" );
	plcol0(1);
	const int num=255;

	int i;
	PLFLT x[256],y[256];
	for (i=0;i<256;i++)
	{
		//x[i]=(double)i;
		x[i]=i-1;
		y[i]=(double)znach[i];
		//g_print("%u:%f;%f|%u\n",i,x[i],y[i],znach[i]);
	}
	plbin(num,x,y,PL_BIN_DEFAULT);

	plend();
}

void plplot_26(){
	plsdev ("xcairo");
	plinit();
	const int num=255;
	int i,max;
	PLFLT x[256],y[256];
	x[0]=-1;
	y[0]=znach[0];
	for (i=1;i<num;i++)
	{
		x[i]=i-1;
		y[i]=y[i-1]+znach[i];
		//g_print("%u;%f:%f\n",i,x[i],y[i]);
	}
	max=y[254];
	plcol0(15);
	plenv(0.0,255.0,1,max+500,0,0);
	plcol0( 3 );
	pllab("Уровни серого для пискселей изображения","Колиесвто пикселей \n (Накопительно)","Камулятивная функция для уровней серого");
	plcol0(1);
	plline(num, x, y );
	plend();
}

void analize(const char* filename)
{
	int i,max;
	for (i=0;i<255;i++) {znach[i]=0;}
	GdkPixbuf *pixbuf;
	GError **tmp= NULL;
	pixbuf=gdk_pixbuf_new_from_file(filename,tmp);
	max=grey(pixbuf);
	//g_print("%u\n",max);
	plplot_05(max);
	plplot_26();
}


void button_clicked(GtkWidget *button, gpointer data) //Кнопка для вызова окна выбора файла.
{
	GtkWidget *dialog;
	GtkFileFilter *filter,*filter2,*filter3;	
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

	filter2 = gtk_file_filter_new();//Создаём фильтр
	gtk_file_filter_set_name(GTK_FILE_FILTER(filter2),"Только png");
	gtk_file_filter_add_mime_type (GTK_FILE_FILTER(filter2),"image/png");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter2);

	filter3 = gtk_file_filter_new();//Создаём фильтр
	gtk_file_filter_set_name(GTK_FILE_FILTER(filter3),"Только jpeg");
	gtk_file_filter_add_mime_type (GTK_FILE_FILTER(filter3),"image/jpeg");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter3);

	res = gtk_dialog_run (GTK_DIALOG (dialog)); //Запускаем
	if (res == GTK_RESPONSE_ACCEPT) { //Если выбор сделан
		const char *filename;//Выбраный файл
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
		filename = gtk_file_chooser_get_filename (chooser);//Путь к файлу
		g_print ("%s\n",filename);
		analize(filename);
	}
	gtk_widget_destroy (dialog);//Закрыть окно get ();
}


int main(int argc, char *argv[])
{
	GtkWidget *vbox,*HboxBatton,*VboxBatton;
    GtkWidget *file_load;
	GtkWidget *window;

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
