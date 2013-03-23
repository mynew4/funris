SET @MAPID := 631;

DELETE FROM `creature` WHERE `map` = @MAPID;
DELETE FROM `creature_addon` WHERE `guid` NOT IN (SELECT `guid` FROM `creature`);
DELETE FROM `creature_formations` WHERE `leaderGUID` NOT IN (SELECT `guid` FROM `creature`);
DELETE FROM `creature_formations` WHERE `memberGUID` NOT IN (SELECT `guid` FROM `creature`);
DELETE FROM `linked_respawn` WHERE `guid` NOT IN (SELECT `guid` FROM `creature`);
DELETE FROM `linked_respawn` WHERE `linkedGuid` NOT IN (SELECT `guid` FROM `creature`);
#ALTER TABLE `waypoint_data` ADD INDEX `idx_id` (`id`) ;
#ALTER TABLE `creature_addon` ADD INDEX `idx_path` (`path_id`) ;

DELETE FROM `waypoint_data` WHERE `id` NOT IN (SELECT `path_id` FROM `creature_addon`);

INSERT INTO `creature` (SELECT * FROM `tdb`.`creature`);
INSERT INTO `creature_addon` (SELECT * FROM `tdb`.`creature_addon`);
INSERT INTO `creature_formations` (SELECT * FROM `tdb`.`creature_formations`);
INSERT INTO `linked_respawn` (SELECT * FROM `tdb`.`linked_respawn`);
REPLACE INTO `smart_scripts` (SELECT * FROM `tdb`.`smart_scripts`);

REPLACE INTO `creature_template` (SELECT * FROM `tdb`.`creature_template` WHERE `entry` IN (SELECT DISTINCT `id` FROM `creature` WHERE `map` = @MAPID));
REPLACE INTO `creature_template_addon` (SELECT * FROM `tdb`.`creature_template_addon` WHERE `entry` IN (SELECT DISTINCT `id` FROM `creature` WHERE `map` = @MAPID));
REPLACE INTO `creature_template` (SELECT * FROM `tdb`.`creature_template` WHERE `entry` IN (SELECT `difficulty_entry_1` FROM `tdb`.`creature_template` WHERE `entry` IN (SELECT DISTINCT `id` FROM `creature` WHERE `map` = @MAPID)));
REPLACE INTO `creature_template` (SELECT * FROM `tdb`.`creature_template` WHERE `entry` IN (SELECT `difficulty_entry_2` FROM `tdb`.`creature_template` WHERE `entry` IN (SELECT DISTINCT `id` FROM `creature` WHERE `map` = @MAPID)));
REPLACE INTO `creature_template` (SELECT * FROM `tdb`.`creature_template` WHERE `entry` IN (SELECT `difficulty_entry_3` FROM `tdb`.`creature_template` WHERE `entry` IN (SELECT DISTINCT `id` FROM `creature` WHERE `map` = @MAPID)));

INSERT INTO `waypoint_data` (SELECT * FROM `tdb`.`waypoint_data`);