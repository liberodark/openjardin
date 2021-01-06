BEGIN TRANSACTION;
CREATE TABLE "taches" (
        `id`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `designation`	varchar(30)
);
INSERT INTO `taches` (id,designation) VALUES (1,'Soil preparation beeching');
INSERT INTO `taches` (id,designation) VALUES (3,'Amendment');
INSERT INTO `taches` (id,designation) VALUES (5,'Seedling');
INSERT INTO `taches` (id,designation) VALUES (6,'Plantations');
INSERT INTO `taches` (id,designation) VALUES (7,'Weeding');
INSERT INTO `taches` (id,designation) VALUES (8,'Harvest');
INSERT INTO `taches` (id,designation) VALUES (9,'Fallow land');
INSERT INTO `taches` (id,designation) VALUES (10,'Mulching');
INSERT INTO `taches` (id,designation) VALUES (11,'Treatment');
INSERT INTO `taches` (id,designation) VALUES (14,'Disease');

CREATE TABLE "plantes" (
        `id`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `designation`	varchar(40),
        `type_lune`	integer,
        `espece`	INTEGER,
        `nom_latin`	varchar(50),
        `commentaires`	varchar(200),
        `semis_printemps`	varchar(24),
        `semis_ete`	varchar(24),
        `semis_automne`	varchar(24)
);
INSERT INTO `plantes` (id,designation,type_lune,espece,nom_latin,commentaires,semis_printemps,semis_ete,semis_automne) VALUES (1,'-',0,114,'N/A','N/A','000000000000000000000000','000000000000000000000000','000000000000000000000000'),
 (3,'Radish de 18 jours',3,91,'Raphanus sativus
L.','','001111000000000111100000','000000110000000000011000','000000000000000000000000'),
 (4,'Dill',1,4,'Anethum graveolens','','011100000000001110000000','000001110000000000000000','000000001100011000000000'),
 (5,'Garlic ',3,1,'0','cultivation with ridging, no manuring','000000000000000111000000','000000000000000000000000','000000001110000001110000'),
 (6,'Celeri',1,22,'Apium graveolens L.','celeri grand vert','011100000000001110000000','000000000000000000000000','000000000000000000000000'),
 (7,'Squash spaguetti',4,36,'Cucurbita pepo L. Spaghetti','Squash spaguetti','011100000000000001110000','000000000000000000000000','000000000000000000000000'),
 (8,'Chicory italian rossa di firenze',1,26,'Cichorium intybus L.','','011100000000001110000000','000000000000000000000000','000000000000000000000000'),
 (9,'Zucchini longue de Nice ',1,37,'Cucurbita moschata','Zucchini longue de Nice ','011100000000001110000000','000000000000000000000000','000000000000000000000000'),
 (10,'Lovage',1,57,'Levisticum officinale','','011100000000001110000000','000000000000000000000000','000000000000000000000000'),
 (11,'Rye',3,104,'Secale cereale L.','Rye ','000000000000000000000000','000000000000000000000000','000000000110000000011000'),
 (12,'Crocus',2,113,'Crocus sativus L.','','000000000000000000000000','000000110000000000000110','000000000000000000000000'),
 (13,'Black current',3,21,'Ribes nigrum','','000000000000000000000000','000000000000000000000000','111000000111000000111000'),
 (14,'Curled Parsley',1,76,'Petroselinum crispum ','','001111000000000001111000','000000000000000000000000','000000000000000000000000'),
 (15,'Tomato San Marzano',4,109,'Solanum lycopersicum L.','Italian variety for sauce - open field without stake','011100000000001110000000','000000000000000000000000','000000000000000000000000'),
 (16,'Eggplant',4,9,'Solanum melongena L.','','011100000000001110000000','000000000000000000000000','000000000000000000000000'),
 (17,'sweet bell pepper',4,85,'Capsicum annuum
L.','','011100000000001110000000','000000000000000000000000','000000000000000000000000'),
 (18,'Carrots Touchon',3,19,'Daucus Carota L.','Early variety with perfectly cylindrical roots straight and half-long, of brilliant reddish orange color and without heart. The Carrot touchon is a Carrot renowned for these gustatory qualities. It is cultivated in an aerated, fresh, well worked soil, even clayey soils can suit it. Compact soils are not recommended, the roots will tend to fork. Prevent against the Carrot fly. ','001111000000000000011110','000000100000000000000001','000000000000000000000000'),
 (20,'Gherkin',1,35,'','','000000000000000000000000','000000000000000000000000','000000000000000000000000'),
 (21,'Artichoke Impérial Star',4,7,'Cynara cardunculus var. scolymus (L.)','Early and vigorous variety.
Round apple with a diameter of about 12 cm. Mild to slightly sweet flavor.','001100000000000000000110','000000000000000000000000','000000000110000110000000'),
 (22,'seasonal salad',0,55,'','','000000000000000000000000','000000000000000000000000','000000000000000000000000'),
 (23,'basil big green',0,12,'','','000000000000000000000000','000000000000000000000000','000000000000000000000000'),
 (24,'wild strawberry',0,49,'','','000000000000000000000000','000000000000000000000000','000000000000000000000000');

CREATE TABLE "observations" (
        `id`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `designation`	varchar(40),
        `date`	varchar(10),
        `type`	INTEGER,
        `commentaires`	INTEGER,
        `id_culture`	INTEGER
);
CREATE TABLE "lune" (
        `id`	INTEGER,
        `designation`	varchar(30)
);
INSERT INTO `lune` (id,designation) VALUES (1,'Feuilles');
INSERT INTO `lune` (id,designation) VALUES (2,'Fleurs');
INSERT INTO `lune` (id,designation) VALUES (3,'Racines');
INSERT INTO `lune` (id,designation) VALUES (4,'Fruits / graines');

CREATE TABLE "familles" (
        `id`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `designation`	varchar(30),
        `couleur`    varchar(10)
);
INSERT INTO `familles` (id,designation,couleur) VALUES (1,'Actinidiaceae','#ded8d7'),
 (2,'Amaranthaceae','#ded8d7'),
 (3,'Apiaceae
(Ombellifères)','#ff5500'),
 (4,'Asteraceae (composées)','#aaaa7f'),
 (5,'Aizoaceae','#ded8d7'),
 (6,'Basellaceae','#ded8d7'),
 (7,'Brassicaceae
(Crucifères)','#aaffff'),
 (8,'Chénopodiaceae','#ff557f'),
 (9,'Convolvulaceae','#ded8d7'),
 (10,'Cucurbitaceae','#ffaa7f'),
 (11,'Ericaceae','#ded8d7'),
 (12,'Fabaceae
(Papilionaceae)','#00ff00'),
 (13,'Grossulariaceae','#ded8d7'),
 (14,'Lamiaceae (Labiées)','#ded8d7'),
 (15,'Alliaceae','#ffff7f'),
 (16,'Asparagaceae
(Liliaceae)','#ded8d7'),
 (17,'Poaceae
(Graminées)','#aaaaff'),
 (18,'Polygonaceae','#ded8d7'),
 (19,'Portulacaceae','#ded8d7'),
 (20,'Rosaceae','#ded8d7'),
 (21,'Solanaceae','#ff00ff'),
 (22,'Valérianaceae','#55ff7f'),
 (23,'Iridacée','#ded8d7'),
 (24,'-','#ded8d7'),
 (25,'Amaryllidaceae','#ded8d7'),
 (26,'Malvaceae','#ded8d7'),
 (27,'Buxaceae','#ded8d7');

CREATE TABLE "especes" (
        `id`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `designation`	varchar(30),
        `famille`	INTEGER,
        `positif`	varchar(200),
        `negatif`	varchar(200),
        `commentaires`	varchar(200),
        `compost`	INTEGER,
        `rotation_ans`	INTEGER,
        `rotation_familles`	varchar(22)
);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (1,'Garlic',15,'Carrot, Strawberry, Tomato','Peas, Beans, chou','favorable aprés fabacées ou Potato',1,6,'0000010000000000010000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (2,'Ligonberry',11,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (3,'Amaranth',2,'','',NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (4,'Dill',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (5,'Angelica',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (6,'Orache',8,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (7,'Artichoke',3,'','','',3,6,'0000000000000000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (8,'Asparagus',16,'Parsley, Tomato','Onion','',0,0,'0000000000000000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (9,'Eggplant',21,'Tarragon, Peas, Thyme','','souvent placé en tête de rotation',3,5,'0000010000000001000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (10,'Bamboo',17,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (11,'Malabar spinach',6,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (12,'Basil',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (13,'Chard',8,'','','',2,5,'1000001000000010000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (14,'Beet',2,'Cabbage, Celerys, Lettuces, Onions','Beanss grimpants','',2,5,'0010100010010010000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (15,'Wheat',17,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (16,'Chard',8,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (17,'Broccoli',7,'Fava bean, Celery, camomille, Dill, Mint, capucine, Onion, Oregano, Potato, Sage, Rosemary','Lettuce, fraise, Tomato',NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (18,'Cardoon',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (19,'Carrot',3,'','','',2,6,'0000001000010100000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (20,'Caraway',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (21,'Cassissier',13,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (22,'Celery',3,'Fava bean, Cabbage, Leek, Onion, Tomato','Lettuce, Parsley','',3,5,'0000001000010000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (23,'Celery root',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (24,'French parsley',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (25,'Chayote',10,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (26,'Chicory',4,'','','',2,5,'0000001000001000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (27,'Brussels sprouts',7,'Fava bean, Celery, Dill, Hyssop, Mint, capucine, Potato, Sage, Rosemary','fraise',NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (28,'Cabbage',7,'Fava bean, Beet, Celery, camomille, Dill, Hyssop, Mint, capucine, Onion, Oregano, Potato, Sage, Rosemary','vigne, fraise, Tomato','',3,5,'0000001000000100000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (29,'Spring onion',15,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (30,'Chive',15,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (31,'Pumpkim',10,'Fava bean, maïs, Mint, capucine, Radish','Potato',NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (32,'Claytone',19,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (33,'Cucumber',10,'Fava bean, Broccoli, Celery, Lettuce, Peas, Radish, Tomato','rue, Sage','',3,5,'0000001000000001000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (34,'Coriander',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (35,'Gherkin',10,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (36,'Squash',10,'Fava bean, maïs, Mint, capucine, Radish','Potato','',3,5,'0000000000000001000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (37,'Zucchini',10,'','','',3,3,'0000001000000001000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (38,'Cress',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (40,'Chinese artichoke',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (41,'Shallot',15,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (42,'Endive',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (43,'Spinash',8,'Eggplant, chou, Celery, Onion, Peas, fraise','','',3,5,'0000001000000010000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (44,'Tarragon',4,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (45,'Fennel',3,'','','',3,5,'0000000000010000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (46,'Fennel large',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (47,'Fava beans',12,'Beets, bourrache, Cabbage, Carrot, chou-fleur, maïs, oeillet d''Inde, Squash, fraise, Tomato','Chive, Fennel, Garlic, Leek',NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (48,'Watercress',7,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (49,'Strawberry',20,'','','',3,5,'0000001100000000100100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (50,'Rasberry',20,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (51,'Gooseberry',13,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (52,'Beans',12,'Beets, bourrache, Cabbage, Carrot, chou-fleur, maïs, oeillet d''Inde, Squash, fraise, Tomato','Chive, Fennel, Garlic, Leek',NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (53,'Hyssop',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (54,'Kiwi',1,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (55,'Lettuce',4,'Beet, chou, trèfle, Peas, Radish, fraise','','',2,5,'0000000000000000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (56,'Lentil',12,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (57,'Lovage',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (58,'Horse parsley',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (59,'Corn salad',22,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (60,'Sweet corn',17,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (61,'Margoram',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (62,'Lemon balm',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (63,'Melon',10,'maïs, Sunflower','','',3,5,'0000000100000001000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (64,'Mint',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (65,'Mustard',7,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (66,'Blueberry',11,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (67,'Turnip',7,'','','',1,4,'0000001110010100000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (68,'Onion',15,'Beet, chou, Carrot, Lettuce, Potato, fraise, Tomato','Fava bean, Beans, Peas','',1,7,'0000010000000000010000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (69,'Barley',17,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (70,'Oregano',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (71,'Sorrel',18,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (72,'Parsnip',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (73,'Watermelon',10,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (74,'Sweet potato',9,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (75,'Pattypan Squash',10,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (76,'Parsley',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (77,'Small peas',12,'Les Peas aiment la proximité du Fennel, des Cucumbers, Lettuces et des Carrots.','','',2,0,'0000000000000000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (78,'Ground Cherry',21,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (79,'Chili Pepper',21,'Basil, Carrot, Margoram, Onions, Oregano','Fennel','',3,5,'0000010000000001000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (80,'Peas',12,'Asparagus, Eggplant, Carrot, Celery, Cucumbers, Lettuce, Radish, épinard, Tomato, Beanss, Turnips','','',1,5,'0000000000000000100000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (81,'Dandelion',3,'',NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (82,'Leek',25,'Carrot, Celery, endive, épinard, Strawberry, Corn salad, œillet d''Inde, Parsnip, Tomato.','Garlic, Asparagus, Chard, Beet, Broccoli, chou, chou de Bruxelles, chou-fleur, Fava bean, Beans, Onion, Parsley, Chili Pepper, Peas, Pepper, Sage.','Parasites   Anguillule, leek moth, thrips, leek worm Maladies   Mildiou, oïdium, rouille',3,6,'0000010000000000010000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (83,'Pearl',8,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (84,'Macer',12,'Artichoke,Broccoli, Carrot, Celery, chou, Cucumber,Squash, spinach, Lettuce, le melon, Mint, navet, Parsnip,  Radish','Garlic, Asparagus, Chive, Eggplant, Onion,Parsley,  Chili Pepper,  Leek,  Potato,  Tomato.','  ',0,0,'0000000000000000000000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (85,'Pepper',21,'','','',3,5,'0000010000000001000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (86,'Potato',21,'Garlic, Beans, Peas, Fava bean, chou, maïs, Lettuce, Onion, pétunia, oeillet d''Inde, Radish, Pumpkin, Zucchinis','pomme, Pumpkim, Tomato','',3,5,'0000000000000000000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (87,'Pumpkin',10,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (88,'Pumpkin',10,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (89,'Purselane',19,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (90,'Quinoa',8,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (91,'Radish',7,'Fava bean, chou, chou-fleur, Cucumber, Lettuce, Peas, Squash, Tomato','vigne, Hyssop',NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (92,'Horseradish',7,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (93,'Broccoli Rabe',7,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (94,'Rhubarb',18,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (95,'Rosemary',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (96,'Blackberry',20,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (97,'Aragula',7,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (98,'Rutabaga',7,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (99,'Salsify',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (100,'Savory',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (101,'Buckwheat',18,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (102,'Sage',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (103,'Black Salsify',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (104,'Rye',17,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (105,'Sorghum',17,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (106,'Stevia',4,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (107,'Tetragon',5,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (108,'Thyme',14,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (109,'Tomato',21,'Asparagus, Basil, Carrot, Onion, Parsley, Peas, Sage, Leek, Radish','Fennel, Potato, chou','',3,5,'0000000000000001000100');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (110,'Cherry Tomato',21,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (111,'Jerusalem Artichoke',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (112,'Sunflower',3,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (113,'Crocus',23,'','','',2,5,'0000000000000000010000');
INSERT INTO `especes` (id,designation,famille,positif,negatif,commentaires,compost,rotation_ans,rotation_familles) VALUES (114,'-',24,'','','',1,1,'0000000000000000000000');
CREATE TABLE "unites" (
	`id`	INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30)
);
CREATE TABLE "type_de_tache" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30),
	`couleur`	varchar(10),
	`forme`	INTEGER
);
CREATE TABLE  "type_de_moyen" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30),
	`commentaires`	varchar(200)
);
CREATE TABLE  "tasks" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30),
	`commentaires`	varchar(200),
	`depart`	varchar(10),
	`fin`	varchar(10),
	`duree`	INTEGER,
	`precedent`	INTEGER,
	`phase_parent`	INTEGER,
	`avancement`	INTEGER,
	`type`	INTEGER,
	`contrainte_date`	INTEGER,
	`id_culture`	INTEGER
);
CREATE TABLE  "ressources" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30),
	`commentaires`	varchar(200),
	`tache`	INTEGER,
	`cout_prevu`	NUMERIC,
	`cout_actualise`	NUMERIC
);

CREATE TABLE  "moyens" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30),
	`commentaires`	varchar(200),
        `cout_unitaire`	NUMERIC,
	`coordonnees`	INTEGER,
	`type`	INTEGER,
	`unite`	INTEGER
);

CREATE TABLE  "liste_ressources" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30),
	`id_task`	INTEGER,
	`id_ressource`	INTEGER
);

CREATE TABLE  "liste_moyens" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	varchar(30),
	`id_ressource`	INTEGER,
	`id_moyen`	INTEGER,
	`quantite_prevu`	NUMERIC,
	`quantite_realise`	NUMERIC,
	`unite`	INTEGER
);

CREATE TABLE "cultures" ( `id` INTEGER PRIMARY KEY AUTOINCREMENT,
`designation` varchar(40),
`parcelle` integer, `date_semis` varchar(10),
`type_plante` integer,
`commentaires` varchar(200),
`etat` INTEGER,
`duree` INTEGER,
`date_recolte` varchar(10) );

CREATE TABLE  "coordonnees" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`societe`	varchar(30),
	`commentaires`	varchar(200),
	`nom`	varchar(30),
	`prenom`	varchar(30),
	`adresse`	varchar(50),
	`code_postal`	varchar(5),
	`ville`	varchar(50),
	`pays`	varchar(20),
	`telephone_fixe`	varchar(20),
	`portable`	varchar(20),
	`fax`	varchar(20),
	`email` varchar(30)
);
INSERT INTO "unites" (id,designation) VALUES (1,'UNIT'),
 (2,'HOURS'),
 (3,'DAYS'),
 (4,'PACKAGE'),
 (5,'KG'),
 (6,'METERS'),
 (7,'M²'),
 (8,'LITRES');

INSERT INTO "type_de_tache" (id,designation,couleur,forme) VALUES (1,'Phases','#00ffff',6),
 (2,'Soil Preparation','#ffaa00',1),
 (3,'Labour','#aa5500',2),
 (4,'Seedbed preparation','#aaaa7f',1),
 (5,'Seeding','#ffff00',8),
 (6,'documentations','#00ff00',1),
 (7,'Fertilization','#00ff00',2),
 (8,'Harvest','#ff5500',1),
 (9,'End of culture','#ff0000',5),
 (10,'Irrigation','#00ffff',5);
INSERT INTO "type_de_moyen" (id,designation,commentaires) VALUES (1,'Staff','Staff'),
 (2,'Matérials',' '),
 (3,'Outside services',' '),
 (4,'Supplies',' ');

CREATE TABLE `parcelles` (
         `id`	INTEGER UNIQUE,
         `designation`	TEXT PRIMARY KEY,
         `mode_affichage` INTEGER
 );
COMMIT;
