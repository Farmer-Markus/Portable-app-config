#include <gtk/gtk.h>
#include <cjson/cJSON.h> 
char conffile[] = "./atlauncher-portable-config.json";

FILE *filwr; //file write
FILE *filr;  //file read
cJSON *config;
int check1;
int check2;
int check3;


int readconf ()
{
  filr = fopen(conffile, "r"); 
  if (filr == NULL)
  {
    g_print ("Error: Unable to open config");
    return 1;
  }
  
  char buffer[1024];
  int configread = fread(buffer, 1, sizeof(buffer), filr);
  fclose(filr);
  
  config = cJSON_Parse(buffer);
  if (config == NULL)
  {
    const char *jsonerror = cJSON_GetErrorPtr();
    if (jsonerror != NULL)
    {
      g_print ("Error reading json config file: %s\n", jsonerror);
    }
    
    cJSON_Delete(config);
    return 1;
  }
  
  cJSON *name = cJSON_GetObjectItemCaseSensitive(config, "Alsa-Sound");
  if (cJSON_IsString(name))
  {
    printf("Name: %s\n", name->valuestring);
  }
  
  cJSON_Delete(config);
  return 0;
}

static void save (GtkWidget *widget, gpointer data)
{
  cJSON *conf = cJSON_CreateObject();
  
  cJSON_AddItemToObject(conf,"config", config = cJSON_CreateObject());
  
  cJSON_AddNumberToObject(config,"Alsa-Sound",check1); 
  cJSON_AddNumberToObject(config,"Internal-jar",check2); 
  cJSON_AddNumberToObject(config,"Noupdates",check3); 
  
  filwr = fopen(conffile, "w"); 
  char *configwrite = cJSON_Print(conf); 
  filwr;
  fputs(configwrite, filwr);
  fclose;
  
  cJSON_free(configwrite);
  cJSON_Delete(config);

  g_print ("saved in '%s'\n", conffile);
}

static void print_cancel (GtkWidget *widget, gpointer data)
{
  g_print ("cancel\n");
}

static void print_import (GtkWidget *widget, gpointer data)
{
  g_print ("import\n");
}

static void checker1 (GtkWidget *checkbox1, gpointer data) //*source
{
  check1 = (gtk_check_button_get_active(GTK_CHECK_BUTTON(checkbox1)));
}

static void checker2 (GtkWidget *checkbox2, gpointer data) //*source
{
  check2 = (gtk_check_button_get_active(GTK_CHECK_BUTTON(checkbox2)));
}

static void checker3 (GtkWidget *checkbox3, gpointer data) //*source
{
  check3 = (gtk_check_button_get_active(GTK_CHECK_BUTTON(checkbox3)));
}

static void gui (GtkApplication* app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *checkbox1;
  GtkWidget *checkbox2;
  GtkWidget *checkbox3;
  GtkWidget *fixed;
  GtkWidget *seperator;
  GtkWidget *label;


  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Configuration");
  gtk_window_set_default_size (GTK_WINDOW (window), 350, 450);
  gtk_window_set_resizable (GTK_WINDOW(window), FALSE);

  fixed = gtk_fixed_new ();
  gtk_window_set_child (GTK_WINDOW (window), fixed);


  seperator = gtk_separator_new (GTK_ORIENTATION_VERTICAL);
  gtk_fixed_put (GTK_FIXED(fixed), seperator, 5, 5);
  gtk_widget_set_size_request (seperator, 470, 440);


  label = gtk_label_new ("(No launcher updates)");
  gtk_fixed_put (GTK_FIXED(fixed), label, 320, 45);


  button = gtk_button_new_with_label ("Save");
  g_signal_connect (button, "clicked", G_CALLBACK (save), NULL);
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_window_destroy), window);
  gtk_fixed_put (GTK_FIXED(fixed), button, 0, 450);
  gtk_widget_set_size_request (button, 80, 35);

  button = gtk_button_new_with_label ("Cancel");
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_window_destroy), window);
  gtk_fixed_put (GTK_FIXED(fixed), button, 400, 450);
  gtk_widget_set_size_request (button, 80, 35);

  button = gtk_button_new_with_label ("Import");
  g_signal_connect (button, "clicked", G_CALLBACK (readconf), NULL);
  gtk_fixed_put (GTK_FIXED(fixed), button, 200, 450);
  gtk_widget_set_size_request (button, 80, 35);


  checkbox1 = gtk_check_button_new_with_label ("Use alsa sound driver");
  gtk_fixed_put (GTK_FIXED(fixed), checkbox1, 10, 10);
  g_signal_connect (checkbox1, "toggled", G_CALLBACK (checker1), NULL);

  checkbox2 = gtk_check_button_new_with_label ("Use internal jar");
  gtk_fixed_put (GTK_FIXED(fixed), checkbox2, 10, 40);
  g_signal_connect (checkbox2, "toggled", G_CALLBACK (checker2), NULL);
  
  checkbox3 = gtk_check_button_new_with_label ("Diable updates");
  gtk_fixed_put (GTK_FIXED(fixed), checkbox3, 10, 70);
  g_signal_connect (checkbox3, "toggled", G_CALLBACK (checker3), NULL);



  gtk_window_present (GTK_WINDOW (window));
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;
  
  readconf();
  
  app = gtk_application_new ("atlauncher-portable.gui", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (gui), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
//gtk_check_button_set_active(GTK_CHECK_BUTTON(checkbox), TRUE);
