#include <gtk/gtk.h>
#include <cjson/cJSON.h>
#include <argp.h>
#include <stdio.h>

const char conffile[] = "atlauncher-portable-config.json";

FILE *filwr; //file write
FILE *filr;  //file read
cJSON *istate1, *istate2, *istate3; //Importet state(checkbox)
cJSON *config;
int state, state1, state2, state3;
void changestate();
void error();

int readconf () {
  cJSON *subconf;
  
  filr = fopen(conffile, "r"); 
  
  if (filr == NULL) {
    g_print ("\x1b[33m" "INFO: Unable to open config\n" "\x1b[0m");
    return 1;
  }
  
  char buffer[1024];
  int configread = fread(buffer, 1, sizeof(buffer), filr);
  fclose(filr);
  
  config = cJSON_Parse(buffer);
  if (config == NULL) {
    g_print ("\x1b[31m" "Error: Unable to read config file\n" "\x1b[0m");
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

  g_print("\x1b[32m" "INFO: Successfully opened config\n" "\x1b[0m");
  return 0;
}

void save (GtkWindow *window, gpointer user_data) {
  window = user_data;
  cJSON *conf;

  if (state1 == 0 && state2 == 0 && state3 == 0 && filr == NULL) {
    gtk_main_quit();
    return;
  }

  conf = cJSON_CreateObject();
  
  cJSON_AddItemToObject(conf,"config", config = cJSON_CreateObject());
  
  cJSON_AddNumberToObject(config, "alsasound", state1); 
  cJSON_AddNumberToObject(config, "internaljar", state2); 
  cJSON_AddNumberToObject(config, "noupdates", state3); 
  
  
  char *configwrite = cJSON_Print(conf);
  filwr = fopen(conffile, "w");

  if (filwr == NULL) {
    g_print ("\x1b[31m" "Error: Unable to create config file\n" "\x1b[0m");
    error(window, "Unable to create config file!");
    return;
  }

  fputs(configwrite, filwr);
  fclose;

  const char *errors = cJSON_GetErrorPtr();
  
  cJSON_free(configwrite);
  cJSON_Delete(config);
  
  g_print ("\x1b[32m" "INFO: Saved in '%s'\n" "\x1b[0m", conffile);
  gtk_main_quit();
}

void checker(GtkWidget *source, gpointer data) {
  const char *btnname;
  
  btnname = gtk_widget_get_name(source);
  state = (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(source)));
  
  
  if ( strcmp(btnname, "checkbox1") == 0 ) {
    state1 = state;
    return;
  }
  
  if ( strcmp(btnname, "checkbox2") == 0 ) {
    state2 = state;
    return;
  }
  
  state3 = state;
}

void import() {
  
}

void gui() {
  GtkBox *box;
  GtkBuilder *builder;
  GObject *window, *button;
  GtkWidget *checkbox1, *checkbox2, *checkbox3;

  char savetooltip[55];
  snprintf(savetooltip, 55, "Saves the config into: %s", conffile);



  builder = gtk_builder_new();
  gtk_builder_add_from_resource(builder, "/ui/main.ui", NULL);

  window = gtk_builder_get_object(builder, "window");
  

  button = gtk_builder_get_object(builder, "btnsave");
  gtk_widget_set_tooltip_text(GTK_WIDGET(button), savetooltip);
  g_signal_connect(button, "clicked", G_CALLBACK(save), window);

  button = gtk_builder_get_object(builder, "btnimport");
  g_signal_connect(button, "clicked", G_CALLBACK(import), NULL);
  gtk_widget_set_tooltip_text(GTK_WIDGET(button), "Not implemented yet");

  button = gtk_builder_get_object(builder, "btncancel");
  g_signal_connect(GTK_WIDGET(button), "clicked", G_CALLBACK(gtk_main_quit), NULL);

  box = GTK_BOX(gtk_builder_get_object(builder, "boxframe"));
  gtk_box_set_spacing(box, 5);


  checkbox1 = gtk_check_button_new_with_label("Use alsa audio driver");
  g_signal_connect(checkbox1, "toggled", G_CALLBACK(checker), NULL);
  gtk_box_pack_start(GTK_BOX(box), checkbox1, FALSE, FALSE, FALSE);
  gtk_widget_set_tooltip_text(checkbox1, "Forces the launcher to use alsa as audio backend(can fix some issues with mods and pipewire)");
  gtk_widget_set_name(checkbox1, "checkbox1");
  gtk_widget_set_visible(checkbox1, TRUE);

  checkbox2 = gtk_check_button_new_with_label("Use internal jar");
  g_signal_connect(checkbox2, "toggled", G_CALLBACK(checker), NULL);
  gtk_box_pack_start(GTK_BOX(box), checkbox2, FALSE, FALSE, FALSE);
  gtk_widget_set_tooltip_text(checkbox2, "Uses ATlauncher binary inside of application without extracting(You won't be able to install updates!)");
  gtk_widget_set_name(checkbox2, "checkbox2");
  gtk_widget_set_visible(checkbox2, TRUE);

  checkbox3 = gtk_check_button_new_with_label("Disable updates");
  g_signal_connect(checkbox3, "toggled", G_CALLBACK(checker), NULL);
  gtk_box_pack_start(GTK_BOX(box), checkbox3, FALSE, FALSE, FALSE);
  gtk_widget_set_name(checkbox2, "checkbox2");
  gtk_widget_set_visible(checkbox3, TRUE);

  
  changestate(checkbox1, checkbox2, checkbox3);
}


void error(GtkWindow *window, char *errmessage) {
  GtkWidget *errdialog;
  GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;

  errdialog = gtk_message_dialog_new(window, flags, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, errmessage, g_strerror(errno));

  gtk_dialog_run(GTK_DIALOG(errdialog));
  gtk_widget_destroy(errdialog);
}


void changestate(GtkWidget *checkbox1, GtkWidget *checkbox2, GtkWidget *checkbox3) {
  if ( readconf() != 0 ) {
    return;
  }
  
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbox1), istate1->valueint);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbox2), istate2->valueint);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbox3), istate3->valueint);
}

static int cmdoptions(int key, char *arg, struct argp_state *state) {
 switch (key){case 'p': printf(".\n"); break;}
 
 return 0;
}

int main() {
  gtk_init(NULL, NULL);
  gui();
  gtk_main();

  return 0;
}
