ALTER TABLE `mail_level_reward`
ADD COLUMN `subject`  varchar(200) NULL DEFAULT NULL AFTER `senderEntry`,
ADD COLUMN `message`  varchar(500) NOT NULL DEFAULT '' AFTER `subject`,
ADD COLUMN `money`  int(11) UNSIGNED NOT NULL DEFAULT 0 AFTER `message`,
ADD COLUMN `item`  int(11) NOT NULL DEFAULT 0 AFTER `money`,
ADD COLUMN `item_count`  int(11) UNSIGNED NOT NULL DEFAULT 0 AFTER `item`;