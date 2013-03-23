ALTER TABLE `character_stats`
ADD COLUMN `ap_multi` int(10) unsigned NOT NULL DEFAULT '0' AFTER `rangedAttackPower`,
ADD COLUMN `ap_mods` int(10) unsigned NOT NULL DEFAULT '0' AFTER `ap_multi`,
ADD COLUMN `pos_resbuffmods` int(10) unsigned NOT NULL DEFAULT '0' AFTER `ap_mods`,
ADD COLUMN `neg_resbuffmods` int(10) unsigned NOT NULL DEFAULT '0' AFTER `pos_resbuffmods`,
ADD COLUMN `mainhandrating` int(10) unsigned NOT NULL DEFAULT '0' AFTER `neg_resbuffmods`,
ADD COLUMN `hasterating` int(10) unsigned NOT NULL DEFAULT '0' AFTER `mainhandrating`,
ADD COLUMN `rangedatttime` int(10) unsigned NOT NULL DEFAULT '0' AFTER `baseatttime`,
ADD COLUMN `minrangeddamage` int(10) unsigned NOT NULL DEFAULT '0' AFTER `maxdamage`,
ADD COLUMN `maxrangeddamage` int(10) unsigned NOT NULL DEFAULT '0' AFTER `minrangeddamage`;