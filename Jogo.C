#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // Para funções do sistema como Sleep() e system("cls")
#include <conio.h>   // Para não precisar dar enter em todo comando
#include <time.h>    // Para gerar números aleatórios 

// Função de imprimir lento
void printl(const char *texto, int atraso_ms)
{
	int i;
	// Imprime o texto caractere por caractere criando um efeito de digitação
	for (i = 0; texto[i] != '\0'; i++)
	{
		printf("%c", texto[i]); 
		fflush(stdout); 
		Sleep(atraso_ms); 
	}
	printf("\n"); 
}

int main()
{
	//setup de linguagem de terminal para o português
	setlocale(LC_ALL, "Portuguese");
	// Variáveis de laço, movimentação e menus
	int i, j;
	char mov = 'w'; 
	char dir = '^';
	int menu = 0;
	char player[50]; 
	
	// Inventário do jogador e Status
	int chaves = 0; 
	int arma = 0; 
	const char *nome_arma = "Nenhuma (Desarmado)";

	// Coordenadas atuais do jogador
	int px = 0; 
	int py = 0;
	
	// Controle de Fases e Limites do Mapa Ativo
	int fase_atual = 1;
	int max_x = 10;
	int max_y = 10;

	// seed do rand
	srand(time(NULL));

	// Matriz do mapa principal
	int mapa[15][15];

	// Molde da Fase 1: Vila (10x10)
	int fase1[10][10] =
	{
		{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
		{ 3, 1, 0, 0, 9, 3, 0, 0, 0, 3 }, 
		{ 3, 0, 0, 0, 0, 3, 0, 0, 0, 3 },
		{ 3, 0, 0, 0, 0, 5, 0, 0, 0, 3 },
		{ 3, 3, 3, 3, 3, 3, 0, 0, 0, 3 },
		{ 3, 0, 0, 8, 8, 0, 0, 6, 0, 3 }, 
		{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
		{ 3, 0, 0, 0, 0, 0, 0, 3, 4, 3 },
		{ 3, 0, 0, 0, 0, 0, 0, 3, 10, 3 }, 
		{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 }
	};

	// Molde da Fase 2: Labirinto (15x15)
	int fase2[15][15] =
	{
		{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
		{ 3, 1, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3 }, 
		{ 3, 3, 3, 0, 3, 0, 3, 3, 3, 0, 3, 0, 3, 0, 3 },
		{ 3, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 3, 0, 3 }, 
		{ 3, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 3 },
		{ 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
		{ 3, 0, 3, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
		{ 3, 0, 3, 0, 3, 0, 0, 0, 2, 0, 0, 0, 0, 0, 3 }, 
		{ 3, 0, 3, 0, 3, 0, 3, 3, 3, 3, 3, 3, 3, 0, 3 },
		{ 3, 0, 0, 0, 3, 0, 3, 0, 0, 0, 0, 0, 3, 0, 3 },
		{ 3, 0, 3, 3, 3, 0, 3, 0, 3, 3, 3, 0, 3, 0, 3 },
		{ 3, 0, 0, 0, 0, 0, 3, 0, 3, 2, 3, 0, 3, 0, 3 }, 
		{ 3, 0, 3, 3, 3, 3, 3, 0, 3, 3, 3, 0, 3, 0, 3 },
		{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 3 }, 
		{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 }
	};

	// Loop do menu principal
	while(menu == 0)
	{
		system("cls");
		printl("Dungeons & Dungeons and more Dungeons\n", 40);

		printl("1-Jogar\n", 20);
		printl("2-Tutorial\n", 20);
		printl("3-Sair\n", 20);

		scanf(" %d", &menu);

		// mensagem de erro
		if(menu != 1 && menu != 2 && menu != 3)
		{
			menu = 0;
			printl("OPCAO INVALIDA\n", 25);
			Sleep(1000); 
		}

		// Início do Jogo
		if(menu == 1)
		{
			system("cls");
			printl("Cerca de 1800, em uma vila esquecida da Romenia --pequena, sombria e fria-- tem ocorrido eventos dos quais tem assombrado os moradores.", 20);
			printl("Qual o nome do seu heroi: ", 20);
			scanf(" %s", player); 

			// Configurações e zeramento do estado inicial ao começar um novo jogo
			fase_atual = 1;
			max_x = 10;
			max_y = 10;
			px = 1; py = 1; dir = 'v';
			chaves = 0;
			arma = 0;
			nome_arma = "Nenhuma (Desarmado)";
			
			// SISTEMA DE VIDA E CHANCES
			int vida = 3;
			int chances = 3;

			// Carrega o molde da Fase 1 para a matriz ativa
			for(i = 0; i < 10; i++)
			{
				for(j = 0; j < 10; j++)
				{
					mapa[i][j] = fase1[i][j];
				}
			}

			// Loop principal
			while(menu == 1)
			{
				system("cls");

				// HUD
				printf("Fase %d | Heroi: %s | Vida: %d/3 | Chances: %d | Chaves: %d\n", fase_atual, player, vida, chances, chaves);
				printf("Arma: %s\n", nome_arma);
				printf("Controles: 'WASD' Mover | 'e' Atacar/Interagir | 'q' Sair\n");
				printf("----------------------------------------------------------\n");

				// Renderiza a matriz ativa na tela
				for(i = 0; i < max_y; i++)
				{
					for(j = 0; j < max_x; j++)
					{
						if(mapa[i][j] == 0)      printf("O "); // Chão
						else if(mapa[i][j] == 1) printf("%c ", dir); // Jogador
						else if(mapa[i][j] == 2) printf("Y "); // Monstro
						else if(mapa[i][j] == 3) printf("* "); // Parede
						else if(mapa[i][j] == 4) printf("D "); // Porta Trancada
						else if(mapa[i][j] == 5) printf("= "); // Porta Aberta
						else if(mapa[i][j] == 6) printf("@ "); // Chave
						else if(mapa[i][j] == 7) printf("# "); // Espinho
						else if(mapa[i][j] == 8) printf("k "); // Caixa
						else if(mapa[i][j] == 9) printf("N "); // NPC
						else if(mapa[i][j] == 10) printf("L "); // Escada
					}
					printf("\n");
				}

				// Captura a ação do usuário no teclado
				mov = getch();

				if(mov == 'q')
				{
					menu = 0;
				}
				// Lógica de ataque e interação
				else if(mov == 'e')
				{
					int alvo_px = px;
					int alvo_py = py;
					int ax, ay, hit_px, hit_py, in_range, escolha;

					// Define qual é o bloco imediatamente à frente do jogador
					if(dir == '<') alvo_px--;
					else if(dir == 'v') alvo_py++;
					else if(dir == '^') alvo_py--;
					else if(dir == '>') alvo_px++;

					// Verifica se é o NPC (9)
					if(alvo_px >= 0 && alvo_px < max_x && alvo_py >= 0 && alvo_py < max_y && mapa[alvo_py][alvo_px] == 9)
					{
						system("cls");
						printf("--- MESTRE CAÇADOR ---\n");
						printf("NPC: Ola, %s! O perigo espreita. Escolha a sua arma:\n\n", player);
						printf("1 - Espada de Prata (Area 3x2 a frente)\n");
						printf("2 - Arco de Abençoado (Area 4x1 a frente)\n");
						printf("3 - Estaca de Madeira / Cajado (Adjacentes)\n");
						printf("\nEscolha (1-3): ");
						scanf(" %d", &escolha);
						
						// armas
						if(escolha == 1) { arma = 1; nome_arma = "Espada de Prata"; }
						else if(escolha == 2) { arma = 2; nome_arma = "Arco de Abençoado"; }
						else if(escolha == 3) { arma = 3; nome_arma = "Cajado de Madeira"; }
						Sleep(1000);
					}
					// Se não for o NPC, executa um ataque
					else
					{
						// Varre um grid 9x9 ao redor do jogador para aplicar o dano da arma
						for(ay = -4; ay <= 4; ay++)
						{
							for(ax = -4; ax <= 4; ax++)
							{
								hit_px = px + ax;
								hit_py = py + ay;
								in_range = 0; // define se a casa analisada toma dano

								// Calcula o alcance com base na arma e na direção atual
								if(arma == 0) // Punho
								{
									if(hit_px == alvo_px && hit_py == alvo_py) in_range = 1;
								}
								else if(arma == 1) // Espada
								{
									if(dir == '^' && ay >= -2 && ay <= -1 && ax >= -1 && ax <= 1) in_range = 1;
									else if(dir == 'v' && ay >= 1 && ay <= 2 && ax >= -1 && ax <= 1) in_range = 1;
									else if(dir == '<' && ax >= -2 && ax <= -1 && ay >= -1 && ay <= 1) in_range = 1;
									else if(dir == '>' && ax >= 1 && ax <= 2 && ay >= -1 && ay <= 1) in_range = 1;
								}
								else if(arma == 2) // Arco
								{
									if(dir == '^' && ax == 0 && ay >= -4 && ay <= -1) in_range = 1;
									else if(dir == 'v' && ax == 0 && ay >= 1 && ay <= 4) in_range = 1;
									else if(dir == '<' && ay == 0 && ax >= -4 && ax <= -1) in_range = 1;
									else if(dir == '>' && ay == 0 && ax >= 1 && ax <= 4) in_range = 1;
								}
								else if(arma == 3) // Cajado
								{
									if(ax >= -1 && ax <= 1 && ay >= -1 && ay <= 1) in_range = 1;
								}

								// Destrói monstros(2) e caixas(8) na área que sofreu dano
								if(in_range == 1 && hit_px >= 0 && hit_px < max_x && hit_py >= 0 && hit_py < max_y)
								{
									if(mapa[hit_py][hit_px] == 8 || mapa[hit_py][hit_px] == 2)
									{
										mapa[hit_py][hit_px] = 0;
									}
								}
							}
						}
					}
				}
				// Lógica de Movimentação do Jogador (WASD)
				else
				{
					int nova_px = px;
					int nova_py = py;

					// Antecipa para qual casa o jogador quer ir e qual direção olhará
					if(mov == 'a') { nova_px--; dir = '<'; }
					else if(mov == 's') { nova_py++; dir = 'v'; }
					else if(mov == 'w') { nova_py--; dir = '^'; }
					else if(mov == 'd') { nova_px++; dir = '>'; }

					// Sistema de Colisão e Interações
					if(nova_px >= 0 && nova_px < max_x && nova_py >= 0 && nova_py < max_y)
					{
						int destino = mapa[nova_py][nova_px];

						// Movimento livre em chãos (0) e portas abertas (5)
						if(destino == 0 || destino == 5)
						{
							mapa[py][px] = 0;  
							px = nova_px;      
							py = nova_py;
							mapa[py][px] = 1;  
						}
						// Pega uma chave (6)
						else if(destino == 6)
						{
							chaves++;          
							mapa[py][px] = 0;  
							px = nova_px;      
							py = nova_py;
							mapa[py][px] = 1;  
						}
						// Destranca uma porta (4) caso use uma chave
						else if(destino == 4)
						{
							if(chaves > 0)
							{
								chaves--; 
								mapa[nova_py][nova_px] = 5; 
							}
						}
						// Avança de fase pisando na escada (10)
						else if(destino == 10)
						{
							if(fase_atual == 1)
							{
								fase_atual = 2;
								max_x = 15;
								max_y = 15;
								px = 1; py = 1; dir = 'v'; 
								
								// Copia a matriz da Fase 2 para a tela do jogo
								for(i = 0; i < 15; i++)
								{
									for(j = 0; j < 15; j++)
									{
										mapa[i][j] = fase2[i][j];
									}
								}
								continue; // Força pular pro topo do laço para desenhar imediatamente
							}
							else if(fase_atual == 2)
							{
								system("cls");
								printl("---- VITORIA ----", 50);
								Sleep(3000);
								menu = 0;
							}
						}
						// Sofre dano 
						else if(destino == 7 || destino == 2) 
						{
							if(destino == 7) vida = 0; // Espinho tira 100% da vida
							else vida--;               // Monstro tira 1 de vida
							
							// Lida com o jogador ficando sem vida
							if(vida <= 0)
							{
								if(chances > 0)
								{
									chances--;
									system("cls");
									printf("\n\n\n\t\t*** VOCE MORREU! CHANCES RESTANTES: %d ***\n", chances);
									Sleep(2000);
									
									// --- SISTEMA DE RESPAWN ---
									vida = 3;
									chaves = 0; // Reseta chaves
									px = 1; py = 1; dir = 'v';
									
									// Recarrega todo o mapa de volta ao formato original
									if(fase_atual == 1)
									{
										for(i = 0; i < 10; i++) for(j = 0; j < 10; j++) mapa[i][j] = fase1[i][j];
									}
									else if(fase_atual == 2)
									{
										for(i = 0; i < 15; i++) for(j = 0; j < 15; j++) mapa[i][j] = fase2[i][j];
									}
								}
								else // Jogo termina caso não tenha mais chances
								{
									system("cls");
									printl("VOCE MORREU!", 30);
									printl("---- GAME OVER ----", 40);
									Sleep(2000); 
									menu = 0; 
								}
							}
						}
					}
				}

				// --- INTELIGENCIA ARTIFICIAL DOS MONSTROS ---
				if(menu == 1) 
				{
					int mi, mj;
					// Procura por todos os monstros (2) no mapa
					for(mi = 0; mi < max_y; mi++)
					{
						for(mj = 0; mj < max_x; mj++)
						{
							if(mapa[mi][mj] == 2)
							{
								int r = rand() % 4; // Sorteio (0 a 3) pra decidir rumo
								int mmx = mj;
								int mmy = mi;

								// Aplica a direção do monstro
								if(r == 0) mmy--;      // Sobe
								else if(r == 1) mmy++; // Desce
								else if(r == 2) mmx--; // Esquerda
								else if(r == 3) mmx++; // Direita

								// Se não bater nas paredes da tela
								if(mmx >= 0 && mmx < max_x && mmy >= 0 && mmy < max_y)
								{
									// Anda com sucesso se o bloco estiver vazio
									if(mapa[mmy][mmx] == 0) 
									{
										mapa[mi][mj] = 0;
										mapa[mmy][mmx] = 12; // Modifica para '12' para não mover 2 vezes no mesmo turno
									}
									// Monstro esbarra no jogador (1) e ataca ativamente
									else if(mapa[mmy][mmx] == 1) 
									{
										mapa[mi][mj] = 12; 
										vida--;            
										
										// Processamento de morte por IA de Monstro (Idêntico ao do jogador)
										if(vida <= 0)
										{
											if(chances > 0)
											{
												chances--;
												system("cls");
												printf("\n\n\n\t\t*** VOCE MORREU! CHANCES RESTANTES: %d ***\n", chances);
												Sleep(2000);
												
												// Reseta o estado do nível e status do jogador
												vida = 3;
												chaves = 0; 
												px = 1; py = 1; dir = 'v';
												
												if(fase_atual == 1)
												{
													for(i = 0; i < 10; i++) for(j = 0; j < 10; j++) mapa[i][j] = fase1[i][j];
												}
												else if(fase_atual == 2)
												{
													for(i = 0; i < 15; i++) for(j = 0; j < 15; j++) mapa[i][j] = fase2[i][j];
												}
												
												mi = max_y; // Quebra o ciclo "for" dos monstros para evitar bugs no novo mapa renderizado
												break;
											}
											else
											{
												system("cls");
												printl("VOCE MORREU!", 30);
												printl("---- GAME OVER ----", 40);
												Sleep(2000);
												menu = 0;
												mi = max_y; 
												break; 
											}
										}
									}
									else // Monstro esbarra em outro elemento e não se move
									{
										mapa[mi][mj] = 12; 
									}
								}
								else // O monstro tentou sair da fase, fica parado
								{
									mapa[mi][mj] = 12;
								}
							}
						}
					}

					// Desfaz o valor temporário '12' transformando os monstros de volta para '2' pro próximo turno
					for(mi = 0; mi < max_y; mi++)
					{
						for(mj = 0; mj < max_x; mj++)
						{
							if(mapa[mi][mj] == 12)
							{
								mapa[mi][mj] = 2;
							}
						}
					}
				}
			} 
		} 
	
		// Tutorial e regras do jogo
		if(menu == 2)
		{
			menu = 0;
			system("cls");
			printf("< Jogador olhando para a esquerda\n^ Jogador olhando para cima\n> Jogador olhando para a direita\nv Jogador olhando para baixo\n* Parede: o jogador nao pode passar\n# Espinho: o jogador morre na hora ao pisar\nk Caixa: o jogador nao pode passar, mas destroi com 'e'\nN NPC: Interaja com 'e' para obter armas!\nD Porta fechada: precisa de chave para abrir\n@ Chave: abre uma porta fechada ao interagir\n= Porta aberta: o jogador pode passar\nL Escada: proxima fase\nY Monstro: Patrulha aleatoriamente (Causa dano)\n\n");
			system("pause");
		}
		
		// Sair
		if(menu == 3)
		{
			system("cls");
			printl("Saindo do jogo... Ate a proxima!", 30);
			break;
		}
	} 

	return 0;
}
