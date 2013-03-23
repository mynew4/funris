UPDATE `item_template` SET `Material`='4' WHERE `entry` IN ('11825', '11826', '15196', '15197', '15198', '15199', '18231', '19030', '19312', '19315', '19325', '21563', '22906', '22916', '34478');
UPDATE `item_template` SET `AllowableRace`='-1' WHERE `entry`='21776';
UPDATE `item_template` SET `displayid`='7629' WHERE `entry`='2730';
UPDATE `item_template` SET `displayid`='67049' WHERE `entry`='23166';
UPDATE `item_template` SET `class`='4' WHERE `entry`='18231';
UPDATE `item_template` SET `displayid`='67267' WHERE `entry`='28297';
UPDATE `item_template` SET `displayid`='67447' WHERE `entry`='29248';
UPDATE `item_template` SET `displayid`='67050' WHERE `entry`='30847';
UPDATE `item_template` SET `displayid`='67267' WHERE `entry`='32053';
UPDATE `item_template` SET `spelltrigger_2`='0' WHERE `entry`='33189';
DELETE FROM `item_template` WHERE `entry`='36942';  ##DEL UNGAMOURNE
UPDATE `item_template` SET `displayid`='63661' WHERE `entry`='37706';
UPDATE `item_template` SET `displayid`='44269' WHERE `entry`='37710';
UPDATE `item_template` SET `spellid_2`='0' WHERE `entry`='43517';
UPDATE `item_template` SET `displayid`='24131' WHERE `entry`='45328';
DELETE FROM `npc_vendor` WHERE `entry` IN ('17249', '24968', '27816', '27817', '33930', '33932', '34645', '34681', '34682', '34683', '34684', '34685', '34783', '34787', '34885', '35132', '35337', '35338', '35340', '35341', '35342', '35343', '35826');
DELETE FROM `npc_vendor` WHERE `item` IN ('32566', '34480');
DELETE FROM `npc_vendor` WHERE `entry` IN ('200045', '200046', '200047', '200048', '200049', '200051', '200052', '200054', '200056', '200057', '200058', '200059', '200060', '200061', '200064', '200075', '200102', '200104', '200104', '200105','200114', '200115','200116', '300314','300362');
DELETE FROM `npc_vendor` WHERE `item` IN ('45366', '45407');
UPDATE `creature_template` SET `npcflag`='16' WHERE `entry` IN ('33615', '18990', '223', '33611', '16745', '11081', '11096', '5811', '5567', '11049', '1466', '2857', '11029', '11026', '3412', '11028', '16668', '18991', '33621', '31238', '957', '1246', '1383', '3008', '4586', '4605', '4609', '5500', '6251', '10266', '10276', '10277', '10278', '11041', '11042', '11044', '11046', '11047', '11065', '11066', '11067', '11068', '11070', '11071', '11083', '11084', '16634', '16643', '16671', '16687', '16740','16741','16742','16743','19248','19249','19250','21209','33609','4605','33610','33614','33617','33608','33612','33639','33679','33616','33618', '33996', '31238');

##818errors
