Aula 04 bddad

Exercicios resolução

SELECT * FROM CD;

SELECT * FROM Musica;

SELECT titulo, datacompra FROM CD;

SELECT DISTINCT titulo, datacompra FROM CD;

SELECT DISTINCT codcd, interprete FROM MUSICA;

SELECT DISTINCT codcd "Código do CD", interprete FROM MUSICA;

SELECT titulo, valorpago, ROUND(((valorpago * 0.23)/1.23),2) AS IVA FROM CD;

SELECT * FROM MUSICA WHERE Codcd = 2;

SELECT * FROM MUSICA WHERE Codcd <> 2;

SELECT * FROM MUSICA WHERE Codcd = 2 AND duracao < 6 AND duracao > 4;

SELECT * FROM MUSICA WHERE Codcd = 2 AND duracao < 6 OR duracao > 4;

SELECT * FROM MUSICA 
    WHERE NrMusica = 1 OR NrMusica = 3 OR NrMusica = 5 OR NrMusica = 6;
-- WHERE NrMusica IN (1,3,5,6)

SELECT * FROM MUSICA WHERE NrMusica <> 1 AND NrMusica <> 3 AND NrMusica <> 5 AND NrMusica <> 6;
-- ... NOT IN (1,3,5,6)

SELECT * FROM MUSICA WHERE interprete LIKE '%Orquestra%';

SELECT * FROM MUSICA WHERE lower(interprete) LIKE '%y%';

SELECT * FROM MUSICA WHERE lower(titulo) LIKE '%dal_';

SELECT * FROM MUSICA WHERE titulo LIKE '%\%%' ESCAPE '\';
-- podemos usar qualquer caracter por exemplo '%@%%' ESCAPE '@'
-- se usarmos 2 vezes o caracter para o escape ele procura com os 2 caracteres
-- exemplo '%\%\%%' ESCAPE '\' ele procura com %%

SELECT * FROM MUSICA WHERE titulo LIKE 'B%' OR titulo LIKE 'D%' OR titulo LIKE 'H%';

SELECT * FROM CD WHERE localcompra IS null;

SELECT * FROM CD WHERE localcompra IS NOT null;

SELECT titulo FROM CD WHERE localcompra = 'FNAC';

SELECT titulo FROM CD WHERE localcompra <> 'FNAC' OR localcompra IS null;

-- D exercicio 1