
// terminal.c

//   This is the kserver part for the virtual terminal support.
//   Initialization routines needs to call this file to setup 
//   the virtual terminal functionalities.

#include <kernel.h>


//todo
//set_terminal_pid
//set_terminal_tid


//todo
//criar um diálogo consistente com o servidor de terminal.
//assim ele pode ser chamado e oferecer vários serviços.

unsigned long 
terminal_dialog ( struct window_d *window, 
                  int msg, 
                  unsigned long long1, 
                  unsigned long long2 ) 
{
    //# todo
    return 0;	
}


/*
 ****************************************************
 * systemSetTerminalWindow:
 * 
 * #importante
 * Configura qual vai ser a janela do terminal virtual.
 * Configurar as variáveis de terminal presentes dentro da janela, pois cada 
 * janela pode ter um terminal. 
 *
 * #importante:
 * >>>> Essa e' a janela que o kernel precisa efetuar o refresh do stdout 
 * associado a ela. Dessa forma, as rotinas da libc precisa apenas escrever
 * em stdout, ficando por conta do kernel efetuar o refresh do arquivo 
 * para dentro da janela. 
 * >>>> Mas esse arquivo deve conter as caracteri'ticas de cada caractere. 
 * 
 */


// #importante
// Configuramos qual será a janela do terminal atual.
// +poderíamos configurar a estrutura do terminal atual.
// +podemos usar as variáveis globais como representação
// do terminal atual.
// >>> Avaliaremos isso. Provavelmente teremos que usar a
// estrutura do terminal atual para efetuatmos qualquer tipo 
// de pintura.
// O sistema poderá ter 8 terminais virtuais tty e 8
// usuários diferentes poderão logar ao mesmo tempo.

void systemSetTerminalWindow ( struct window_d *window ){

	// Obs: ?? Como faremos para pintar dentro da janela do terminal.
	// Obs: a rotina de configuração do terminal deverá ajustar as margens 
	// usadas pela rotina de impressão de caracteres.

	// obs:
	// essa modificação de margens obriga o aplicativo a 
	// configurar o terminal somente na hora que for usa-lo,
	// para não correr o risco de imprimir no lugar errado.

//check_window:

    if ( (void *) window == NULL )
        return;

    if ( window->used != 1 || window->magic != 1234 )
        return;
		
       
    // #bugbug
	// De que janela estamos falando, de qual dos terminais virtuais?
	// Pode ser da que esta em primeiro plano.
	// Configurando a variável global que diz qual é 
	// o ID da janela que tem o terminal virtual ativo.

    terminal_window = (int) window->id;

	//configura o status do terminal dentro da janela
	//validade e reusabilidade das variáveis de terminal 
	//dentro da estrutura de janela.

    window->terminal_used = 1;
    window->terminal_magic = 1234; 

	//tab
	//número da tab.
	//indica qual tab o terminal está usando.
	//@todo:
	// Criar uma forma de contar as tabs de terminal 
	// dentro do gerenciador de terminais.
    window->terminal_tab = 0; 


	//rect
	//configura por último
    window->teminal_left = 0;
    window->teminal_top = 0;
    window->teminal_width = 0;
    window->teminal_height = 0;
	//..

	//
	// rcClient  (retângulo)
    //
	
	// test:
	// tentando ajustar as margens para as rotinas de impressão.
	// para que as rotinas de impressão imprimam dentro das 
	// dimensões do terminal. 
	// obs: @todo: Essas margens deverão ser reconfiguradas 
	// quando o terminal é fechado.

	// #bugbug:
	// #importante		
	// Esse ajuste pode significar problemas.

    if ( (void *) window->rcClient !=  NULL )
    {
		//x inicial
        if ( window->rcClient->left > 0 )
        {
            g_cursor_x = (window->rcClient->left/8 ); 
            g_cursor_left = g_cursor_x;

            window->teminal_left = window->rcClient->left; 
        }

		//y inicial
        if ( window->rcClient->top > 0 )
        {
            g_cursor_y = (window->rcClient->top/8 ); 
            g_cursor_top = g_cursor_y;

            window->teminal_top = window->rcClient->top; 
        }
    };

    if ( window->rcClient->width > 0 )
    {
        window->teminal_width = window->rcClient->width; 
    }

    if ( window->rcClient->height > 0 )
    {
        window->teminal_height = window->rcClient->height;
    }

    if ( window->rcClient->right > 0 )
    {
	    // margem direita dada em linhas
        g_cursor_right = window->rcClient->right /8;

        window->teminal_right = window->rcClient->right; 
    }

    if ( window->rcClient->bottom > 0 )
    {
	    // margem inferior dada em linhas
        g_cursor_bottom = window->rcClient->bottom /8;

        window->teminal_bottom = window->rcClient->bottom; 
    }

	//limits
	//@todo: corrigir.
	// ajustes temporários caso tenha havido um erro anteriormente...

    if ( g_cursor_left > 800 )
	{
        g_cursor_left = 795;
    }

    if ( g_cursor_top > 600 )
	{
         g_cursor_top = 595;
    }

    if ( g_cursor_right > 800 )
	{
         g_cursor_right = 795;
    }

    if ( g_cursor_bottom > 600 )
	{
         g_cursor_bottom = 595;
    }

	//
    // Cursor.
    //
	
	g_cursor_width = 8; 
    g_cursor_height = 8; 
    g_cursor_color = COLOR_TERMINALTEXT;
	
	
	//
	// Terminal struct
	//
	
	// #todo
	// Se a estrutura do terminal atual for válida,
	// então vamos configurá-la.
	
	/*
	if ( current_terminal < 0 || current_terminal > 7 )
	{
	    kprintf ("systemSetTerminalWindow: current_terminal fail");
		die ();
	}
	*/
	
	
	if ( (void *) CurrentTTY != NULL )
	{
	    if ( CurrentTTY->used == 1 && CurrentTTY->magic == 1234 )
		{
		    CurrentTTY->terminal_id = current_terminal;
			
		    current_tty = CurrentTTY->index;
			
			
			CurrentTTY->window = window;
	        CurrentTTY->left = window->rcClient->left;
	        CurrentTTY->top = window->rcClient->top;
	        CurrentTTY->width = window->rcClient->width;
	        CurrentTTY->height = window->rcClient->height;
	
            //cursor support.
            CurrentTTY->cursor_x = g_cursor_x;
            CurrentTTY->cursor_y = g_cursor_y;
            CurrentTTY->cursor_width = g_cursor_width;    //??
            CurrentTTY->cursor_height = g_cursor_height;   //??
            CurrentTTY->cursor_color = g_cursor_color;
            CurrentTTY->cursor_left = g_cursor_left;     // margem esquerda dada em linhas
            CurrentTTY->cursor_top = g_cursor_top;      // margem superior dada em linhas
            CurrentTTY->cursor_right = g_cursor_right;    // margem direita dada em linhas
            CurrentTTY->cursor_bottom = g_cursor_bottom; // margem inferior dada em linhas			
			
		}
	}

    // continua ...
}


// o ID da janela que tem o terminal virtual ativo.
// isso deve ir para kgws

int systemGetTerminalWindow (void){

    return (int) terminal_window;
}


//@todo: precisamos de argumentos.
//configuramos o retângulo do terminal virtual corrente.. 
void 
systemSetTerminalRectangle ( unsigned long left,
                             unsigned long top,
							 unsigned long width,
							 unsigned long height )
{
	
	//
	// current terminal
	//

}


// initialize terminal support.

int terminalInit (void){

	// seleciona o primeiro terminal
	current_terminal = 0;
		
	// Continua ...
    return 0;
}



