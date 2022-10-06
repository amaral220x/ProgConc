# multimatriz
Multiplicador de Matrizes em C feito para matéria de Programação Concorrente!

Anotando resultados:
Matriz 500x500 = Multiplicação = (0,449292+0,435822+ 0,434616) / 3 = 0,43991 Total = (2,160949 + 1,298406+ 1,244312)/3 = 1,567889
Matriz 1000x1000 = (3,701855 + 4,071212 + 3,840664) / 3 ~= 3,87124  Total = (6,826504 + 7,185286 + 6,981318)/3 ~= 6,997702
Matriz 2000x2000 = (44,959162 + 43,935868 + 43,694503) / 3 = 44,196511 Total = (57,825528 + 56,665063 + 56,643811)/3 ~= 57,0448006

Prevendo ganhos (TempoSequencial/TempoConcorrente) em um cenário com 4 threads: 

500x500 =   1,567889 / 1,127979 + 0,1099775  = 1,567889 / 1,2379565 = 1,266
1000x1000 = 6,997702 / 3,126462 + 0,96781 = 6,97702 / 4,094272 = 1,70
2000x2000 = 57,0448006 / 12,8482896 + 11,04912775 = 57,0448006 / 23,89741735 = 2,387

Resultados com concorrência 4 Threads

500 x 500
Matriz Sequencial 500x500 (Novos resultados tirados do pc do lab) = (0,669219 + 0,675186 + 0,665827) / 3 = 0,670077
Matriz Concorrente 500x500 = Total (0,205251+ 0,206180 + 0,205937) / 3 = 0,205789
Ganhos = (0,670077 / 0,205789) = 3,25614

1000x1000
Matriz Sequencial 1000x1000 (Tirados do pc do lab) = (6,799534 + 6,973994+ 6,913537) / 3 = 6,89569
Matriz Concorrente 1000x1000 = (1,887944 + 1.985463 + 1,872616)/3 = 1,915341
Ganhos = (6,89569/1,915341) = 3,60024

2000x2000
Matriz Sequencial 2000x2000 (tirados do lab) = (65,424711 + 66,236770+ 66,414173 )/3  = 66,035218
Matriz Concorrente 2000x2000 = (17,160827 + 17,772631 + 17,290619)/3 = 17,41003
Ganhos = (66,035218/17,41003) = 3,79294

Recalculando o teorico

500x500 
Media do Tempo da Mult (0.663333 + 0.666682 + 0.657079) / 3 = (0,662365/4) + 0,002103 (tempo medio restante) = 0,16769425
Ganho teorico = (0,670077/0,16769425) = 3,99583

1000x1000
Media do tempo da mult = (6,815647 + 6,918307 + 7,052986)/3) = (6,92898/4) + 0,008021 (tempo medio restante)  = 1,740266 
Ganho teorico = 6,89569/1,740266 = 3,96243


2000x2000 
Media do tempo da mult = (66,835744 + 64,785539 + 64,264749 )/3 = (65,295344/4) + 0,02936 (tempo medio restante) = 16,353196
Ganho teorico = 66,035218/16,353196 = 4,03806