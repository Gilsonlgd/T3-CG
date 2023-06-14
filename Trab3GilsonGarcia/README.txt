Trabalho de Gilson Garcia.

Este é um jogo de tiro entre naves espaciais.
Sua nave é limitada pelas curvas à sua direita e sua esquerda.
Faça o máximo de pontos que conseguir!

###### Funcionalidades: ######
	Todos os requisitos mínimos foram implementados:
	- Desenho de um cenário com pelo menos 10x a altura da window -> é gerado proceduralmente;
	- As curvas devem ser desenhadas com Bezier ou B-spline -> B-spline nas paredes e Bezier no escudo
	- A nave deve poder disparar contra os inimigos e vice-versa;
	- Deve ter um placar de pontuação;
	- A geometria da nave pode ser bem simples, como uma seta. A geometria dos inimigos;
	pode ser círculos, quadrados, etc;
	- Controle de FPS -> limitado a 60 fps. A movimentação no jogo é calculada e multiplicada por um deltaTime, dado pelo controlador de FPS.

	Requisitos Extra:
	- Poder especial -> escudo;
	- Aumento progressivo da dificuldade com base na pontuação;
	- Controle de vidas do player;
	- Período invencível ao perder vida;
	- Tiros dos inimigos são direcionados ao jogador;
	- Aparência visual (timer, controlador de vidas, estrelas de fundo, telas inicial e final).

Obs.: Acontece um problema quando se move ou redimensiona a janela durante o jogo.
	  Isso acontece porque o calculo do deltaTime é baseado no tempo entre um frame e outro.
	  Quando há redimensionamento da janela, o render é parado, e quando retorna, o deltaTime é muito grande.
	  Isso faz com que vários pontos de controle saiam da tela ao mesmo tempo, e sejam jogados para o início juntos, deformando as curvas.
	  Não encontrei uma forma de implementar o controle de FPS e evitar este problema.
	  Basta rodar o jogo de novo e evitar de redimensionar ou mover a janela durante o jogo. 


###### Intruções: ######
- Movimentação:
	Sua nave está fixa à borda inferior da tela e você só pode controlar sua movimentação para a esquerda e para a direita,
e a velocidade na qual avança pelo mapa.
	- Setas <- e -> transladam no eixo x
	- Setas cima e baixo aumentam e diminuem sua velocidade respectivamente.

- Tiro:
	Você pode atirar ilimitadamente. Seus tiros seguem em linha reta até encontrar um inimigo.
	- Utilize SPAÇO para atirar

- Inimigos:
	São spawnados em ordas, de acordo com a dificuldade atual do jogo.
	Aparecem de acordo com um intervalo de tempo.
	Atiram de acordo com um intervalo de tempo.
	Inimigos atiram na sua direção se você estiver no campo de alcance deles, então esteja pronto para desviar. (EXTRA)

- Vidas (EXTRA):
	Você tem um número máximo de vidas durante o jogo. Seu número inicial de vidas é 5.
	Ao perder uma vida, você muda de cor e fica invencível por 3 segundos, não podendo sofrer dano durante esse período.

	Você perde vidas se:
	- Encostar em uma das curvas delimitadoras.
	- Colidir com o tiro de um inimigo.
	- Colidir com um inimigo.

	Ao chegar em 0 vidas, você perde o jogo.

- Proteção (EXTRA):
	Existe um poder especial! Você pode usar um escudo para evitar danos de tiros e de colisão com outras naves.
	
	Para usar o escudo:
	- Pressione a tecla CTRL.

	O escudo ficará ativo por 3 segundos.
	O escudo tem um tempo de recarga de 13 segundos, use com cuidado.

- Pontuação:
	Você só pontua eliminando inimigos. 
	Para cada inimigo eliminado você ganha 10 pontos.
	Eliminar inimigos com seu escudo não aumenta seu score.

- Progresso e dificuldade (EXTRA):
	Ao acumular pontos, o jogo se torna progressivamente mais difícil.

	O que fica mais difícil:
	- A cada 100 pontos diminui o intervalo entre as ordas de inimigos em 1 segundo, até um mínimo de 3 segundos.
	- A cada 100 pontos diminui o intervalo entre os tiros dos inimigos em 0.5 segundo, até um mínimo de 1 segundo.
	- A cada 300 pontos aumenta o número máximo de inimigos por orda em 1 até um máximo de 10 inimigos.


 