#include "meufiltro.h"

void inicializarWidgetsMeuFiltro() {
	//widgets das opcoes de filtro
	widgetLabelContTamanho=  gtk_label_new("Tamanho da tira");
	widgetControleTamanho = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 80, 1);
	
	widgetLabelContDistancia= gtk_label_new("Distancia das tiras");
	widgetControleDistancia = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 80, 1);

	widgetcheckhorizontal = gtk_check_button_new_with_label("Linhas horizontais");
	widgetcheckverticais = gtk_check_button_new_with_label("Linhas verticais");
	widgetchecktexture = gtk_check_button_new_with_label("Aplicar textura");

	widgetLabelCorselect= gtk_label_new("Selecionar cor da tira");
	widgetColorpicker = gtk_color_button_new();

	g_signal_connect(G_OBJECT(widgetControleTamanho), "value-changed", G_CALLBACK(funcaoAplicar), NULL);
	g_signal_connect(G_OBJECT(widgetControleDistancia), "value-changed", G_CALLBACK(funcaoAplicar), NULL);
}

void adicionarWidgetsMeuFiltro(GtkWidget *container) {

	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 20);
	gtk_container_add(GTK_CONTAINER(container), vbox);

	gtk_container_add(GTK_CONTAINER(vbox), widgetLabelContTamanho);
	gtk_container_add(GTK_CONTAINER(vbox), widgetControleTamanho);

	gtk_container_add(GTK_CONTAINER(vbox), widgetLabelContDistancia);
	gtk_container_add(GTK_CONTAINER(vbox), widgetControleDistancia);

	gtk_container_add(GTK_CONTAINER(vbox), widgetLabelCorselect);
	gtk_container_add(GTK_CONTAINER(vbox), widgetColorpicker);

	gtk_container_add(GTK_CONTAINER(vbox), widgetcheckhorizontal);
	gtk_container_add(GTK_CONTAINER(vbox), widgetcheckverticais);
	gtk_container_add(GTK_CONTAINER(vbox), widgetchecktexture);
}



Imagem meuFiltro(Imagem origem, Imagem textura) {
	int i, j;
	int tamanho = (int) gtk_range_get_value(GTK_RANGE(widgetControleTamanho));
	int distancia = (int) gtk_range_get_value(GTK_RANGE(widgetControleDistancia));

	GdkColor color;
	gtk_color_button_get_color(GTK_COLOR_BUTTON(widgetColorpicker), &color);

	double corred = color.red;
	double corblue = color.green;
	double corgreen = color.blue;

	printf("corred: %lf, corblue: %lf,corgreen: %lf ", corred, corblue, corgreen );
	Imagem destino;

	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgetcheckhorizontal))) {
		// printf("horizontal true \n");
		destino = alocarImagemDimensao(origem.w*2, origem.h, origem.numCanais);
		for(j = 0; j < destino.w; j++) {
			for(i = 0; i < destino.h; i++) {
				destino.m[i][j][0] = 0;
				destino.m[i][j][1] = 0;
				destino.m[i][j][2] = 0;
			}
		}
		for(j = 0; j < destino.w; j=j+tamanho+distancia) {
			for(i = 0; i < destino.h; i++) {
				for (int c=j; c<j+tamanho; c++){
					if(c<destino.w){
						destino.m[i][c][0] = origem.m[i][c/2][0];
						destino.m[i][c][1] = origem.m[i][c/2][1];
						destino.m[i][c][2] = origem.m[i][c/2][2];
					}
				}
				for (int c=j+tamanho; c<j+tamanho+distancia; c++){
					if(c<destino.w){
						destino.m[i][c][0] = corred;
						destino.m[i][c][1] = corblue;
						destino.m[i][c][2] = corgreen;
					}
					
				}
				
			}
		}
	}else{
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgetcheckverticais))){
			// printf("vertical true");
			destino = alocarImagemDimensao(origem.w, origem.h*2, origem.numCanais);
			for(j = 0; j < destino.w; j++) {
				for(i = 0; i < destino.h; i=i+tamanho+distancia) {
					for (int c=i; c<i+tamanho; c++){
						if(c<destino.w){
							destino.m[c][j][0] = origem.m[c][j][0];
							destino.m[c][j][1] = origem.m[c][j][1];
							destino.m[c][j][2] = origem.m[c][j][2];
						}else if(c>destino.w){
							destino.m[c][j][0] = origem.m[c/2][j][0];
							destino.m[c][j][1] = origem.m[c/2][j][1];
							destino.m[c][j][2] = origem.m[c/2][j][2];
						}
					}
					for (int c=i+tamanho; c<i+tamanho+distancia; c++){
						if(c<destino.w){
							destino.m[c][j][0] = corred;
							destino.m[c][j][1] = corblue;
							destino.m[c][j][2] = corgreen;
						}
					}
				}
			}

		}else{
			destino = alocarImagem(origem);
			for(j = 0; j < destino.w; j++) {
				for(i = 0; i < destino.h; i++) {
					destino.m[i][j][0] = origem.m[i][j][0];
					destino.m[i][j][1] = origem.m[i][j][1];
					destino.m[i][j][2] = origem.m[i][j][2];
				}
		}
	}
	}
	int a, b;
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgetchecktexture))) {

		for(j=0;j<destino.w; j=j+a ){
			for(i=0; i<destino.h; i=i+b) {
				// printf(" i:%d textura.h: %d\n",  i, textura.h);
				for(a=0; a<textura.w; a++){
					for (b=0; b<textura.h; b++){
						if(b+i<destino.h && a+j<destino.w){
							destino.m[b+i][a+j][0] = (textura.m[b][a][0])*0.3+(destino.m[b+i][a+j][0])*0.7;
							destino.m[b+i][a+j][1] = (textura.m[b][a][1])*0.3+(destino.m[b+i][a+j][1])*0.7;
							destino.m[b+i][a+j][2] = (textura.m[b][a][2])*0.3+(destino.m[b+i][a+j][2])*0.7;
						}
					}
				}
			}
		}
	}

	return destino;
}