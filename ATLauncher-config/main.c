#include <gtk/gtk.h>
#include <cjson/cJSON.h>

char conffile[] = "./atlauncher-portable-config.json";

FILE *filwr; //file write
FILE *filr;  //file read
cJSON *istate1, *istate2, *istate3; //Importet state(checkbox)
cJSON *config;
int state, state1, state2, state3;
void changestate();


int readconf ()
{
  cJSON *subconf;
  
  filr = fopen(conffile, "r"); 
  
  if (filr == NULL)
  {
    g_print ("\x1b[33m" "INFO: Unable to open config\n" "\x1b[0m");
    return 1;
  }
  
  char buffer[1024];
  int configread = fread(buffer, 1, sizeof(buffer), filr);
  fclose(filr);
  
  config = cJSON_Parse(buffer);
  if (config == NULL)
  {
    g_print ("\x1b[31m" "Error: Cannot read config file\n" "\x1b[0m");
    cJSON_Delete(config);
    return 1;
  }
  
  subconf = cJSON_GetObjectItem(config,"config");
  
  istate1 = cJSON_GetObjectItemCaseSensitive(subconf, "alsasound");
  istate2 = cJSON_GetObjectItemCaseSensitive(subconf, "internaljar");
  istate3 = cJSON_GetObjectItemCaseSensitive(subconf, "noupdates");
  
  istate1->valueint;
  istate2->valueint;
  istate3->valueint;
  
  cJSON_Delete(config);
  g_print ("INFO: Successfully opened config\n");
  return 0;
}

void save (GtkWidget *widget, gpointer data)
{
  cJSON *conf;

  
  conf = cJSON_CreateObject();
  
  cJSON_AddItemToObject(conf,"config", config = cJSON_CreateObject());
  
  cJSON_AddNumberToObject(config, "alsasound", state1); 
  cJSON_AddNumberToObject(config, "internaljar", state2); 
  cJSON_AddNumberToObject(config, "noupdates", state3); 
  
  
  char *configwrite = cJSON_Print(conf);
  filwr = fopen(conffile, "w");  
  filwr;
  fputs(configwrite, filwr);
  fclose;
  
  cJSON_free(configwrite);
  cJSON_Delete(config);
  
  g_print ("INFO: Saved in '%s'\n", conffile);
}

void checker (GtkWidget *source, gpointer data)
{
  const char *btnname;
  
  btnname = gtk_widget_get_name(source);
  state = (gtk_check_button_get_active(GTK_CHECK_BUTTON(source)));
  
  
  if ( strcmp(btnname, "checkbox1") == 0 )
  {
    state1 = state;
    return;
  }
  
  if ( strcmp(btnname, "checkbox2") == 0 )
  {
    state2 = state;
    return;
  }
  
  state3 = state;
}

void gui (GtkApplication* app, gpointer user_data)
{
  GtkWidget *mainwindow;
  GtkWidget *button1, *button2, *button3;
  GtkWidget *checkbox1, *checkbox2, *checkbox3;
  GtkWidget *fixed;
  GtkWidget *seperator1, *seperator2, *seperator3, *seperator4;
  GtkWidget *label;


  mainwindow = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (mainwindow), "Configuration");
  gtk_window_set_default_size (GTK_WINDOW (mainwindow), 480, 485);
  gtk_window_set_resizable (GTK_WINDOW(mainwindow), FALSE);

  fixed = gtk_fixed_new ();
  gtk_window_set_child (GTK_WINDOW (mainwindow), fixed);

  seperator1 = gtk_separator_new (GTK_ORIENTATION_VERTICAL);
  gtk_fixed_put (GTK_FIXED(fixed), seperator1, 5, 5);
  gtk_widget_set_size_request (seperator1, 2.5, 440);
  gtk_widget_set_name (seperator1, "seperator");
  
  seperator2 = gtk_separator_new (GTK_ORIENTATION_VERTICAL);
  gtk_fixed_put (GTK_FIXED(fixed), seperator2, 473.5, 5);
  gtk_widget_set_size_request (seperator2, 2.5, 440);
  gtk_widget_set_name (seperator2, "seperator");
  
  seperator3 = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_fixed_put (GTK_FIXED(fixed), seperator3, 7, 5);
  gtk_widget_set_size_request (seperator3, 466, 2.5);
  gtk_widget_set_name (seperator3, "seperator");
  
  seperator4 = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_fixed_put (GTK_FIXED(fixed), seperator4, 7, 443);
  gtk_widget_set_size_request (seperator4, 466, 2.5);
  gtk_widget_set_name (seperator4, "seperator");
  
  
  GtkCssProvider *provider = gtk_css_provider_new ();
  gtk_css_provider_load_from_data (provider, "#seperator { color: black; }", -1);
  gtk_style_context_add_provider (gtk_widget_get_style_context (seperator1),
  GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_style_context_add_provider (gtk_widget_get_style_context (seperator2),
  GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_style_context_add_provider (gtk_widget_get_style_context (seperator3),
  GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_style_context_add_provider (gtk_widget_get_style_context (seperator4),
  GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  
  

  label = gtk_label_new ("(No launcher updates)");
  gtk_fixed_put (GTK_FIXED(fixed), label, 320, 45);


  button1 = gtk_button_new_with_label ("Save");
  g_signal_connect (button1, "clicked", G_CALLBACK (save), NULL);
  g_signal_connect_swapped (button1, "clicked", G_CALLBACK (gtk_window_destroy), mainwindow);
  gtk_fixed_put (GTK_FIXED(fixed), button1, 0, 450);
  gtk_widget_set_size_request (button1, 80, 35);
  gtk_widget_set_name (button1, "button");

  button2 = gtk_button_new_with_label ("Cancel");
  g_signal_connect_swapped (button2, "clicked", G_CALLBACK (gtk_window_destroy), mainwindow);
  gtk_fixed_put (GTK_FIXED(fixed), button2, 400, 450);
  gtk_widget_set_size_request (button2, 80, 35);
  gtk_widget_set_name (button2, "button");

  button3 = gtk_button_new_with_label ("Import");
  g_signal_connect (button3, "clicked", G_CALLBACK (readconf), NULL);
  gtk_fixed_put (GTK_FIXED(fixed), button3, 200, 450);
  gtk_widget_set_size_request (button3, 80, 35);
  gtk_button_set_has_frame (GTK_BUTTON(button3), TRUE);
  gtk_widget_set_name (button3, "button");
  
  
  GtkCssProvider *btn_provider = gtk_css_provider_new ();
  gtk_css_provider_load_from_data (btn_provider, "#button { background: alpha(currentColor, 0.05); } #button:hover { background-color: rgba(255, 255, 255, 0.1); }", -1); //border: 1px solid black;
  
  gtk_style_context_add_provider (gtk_widget_get_style_context (button1),
  GTK_STYLE_PROVIDER (btn_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_style_context_add_provider (gtk_widget_get_style_context (button2),
  GTK_STYLE_PROVIDER (btn_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_style_context_add_provider (gtk_widget_get_style_context (button3),
  GTK_STYLE_PROVIDER (btn_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  
  //gtk_css_provider_load_from_data (btn_provider, "#button1 { border: 1px solid black; }", -1);



  checkbox1 = gtk_check_button_new_with_label ("Use alsa sound driver");
  gtk_fixed_put (GTK_FIXED(fixed), checkbox1, 10, 10);
  g_signal_connect (checkbox1, "toggled", G_CALLBACK (checker), NULL);
  gtk_widget_set_name (checkbox1, "checkbox1");

  checkbox2 = gtk_check_button_new_with_label ("Use internal jar");
  gtk_fixed_put (GTK_FIXED(fixed), checkbox2, 10, 40);
  g_signal_connect (checkbox2, "toggled", G_CALLBACK (checker), NULL);
  gtk_widget_set_name (checkbox2, "checkbox2");
  
  checkbox3 = gtk_check_button_new_with_label ("Diable updates");
  gtk_fixed_put (GTK_FIXED(fixed), checkbox3, 10, 70);
  g_signal_connect (checkbox3, "toggled", G_CALLBACK (checker), NULL);
  gtk_widget_set_name (checkbox3, "checkbox3");
  
  changestate(checkbox1, checkbox2, checkbox3);
  
  gtk_window_present (GTK_WINDOW (mainwindow));
}

void changestate (GtkWidget *checkbox1, GtkWidget *checkbox2, GtkWidget *checkbox3)
{
  if ( readconf() != 0 )
  {
    return;
  }
  
  gtk_check_button_set_active(GTK_CHECK_BUTTON(checkbox1), istate1->valueint);
  gtk_check_button_set_active(GTK_CHECK_BUTTON(checkbox2), istate2->valueint);
  gtk_check_button_set_active(GTK_CHECK_BUTTON(checkbox3), istate3->valueint);
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;


  app = gtk_application_new ("atlauncher-portable.gui", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (gui), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
