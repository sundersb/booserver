/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP DATABASE IF EXISTS `booserver`;
CREATE DATABASE IF NOT EXISTS `booserver` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `booserver`;

--> Edit these lines to add users:

-- Don't forget to change the passwords. Set user names as you wish.
grant select on booserver.* to 'booserver'@'localhost' identified by 'server_password';

-- Add as much clients as needed:
grant all privileges on booserver.* to 'user'@'%' identified by 'client_password';
--grant all privileges on booserver.* to 'other_user'@'%' identified by 'other_clients_password';

--> End user-editable section

DROP TABLE IF EXISTS `doctor`;
CREATE TABLE IF NOT EXISTS `doctor` (
  `doctor_id` int(11) NOT NULL AUTO_INCREMENT,
  `profile_id` int(11) NOT NULL DEFAULT '0',
  `name` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `fullname` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `study` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `active` bit(1) NOT NULL DEFAULT b'1',
  `duty` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `institute` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `certificate` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `category` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`doctor_id`)
) ENGINE=InnoDB AUTO_INCREMENT=179 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*!40000 ALTER TABLE `doctor` ENABLE KEYS */;

DROP TABLE IF EXISTS `profile`;
CREATE TABLE IF NOT EXISTS `profile` (
  `profile_id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `nline` int(11) DEFAULT NULL,
  `active` bit(1) NOT NULL DEFAULT b'1',
  PRIMARY KEY (`profile_id`)
) ENGINE=InnoDB AUTO_INCREMENT=33 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*!40000 ALTER TABLE `profile` ENABLE KEYS */;

DROP TABLE IF EXISTS `rule`;
CREATE TABLE IF NOT EXISTS `rule` (
  `rule_id` int(10) NOT NULL AUTO_INCREMENT,
  `doctor_id` int(10) DEFAULT '0',
  `nline` int(10) unsigned NOT NULL DEFAULT '0',
  `name` varchar(50) DEFAULT NULL,
  `title` varchar(50) DEFAULT 'no title',
  `dfrom` date DEFAULT NULL,
  `dtill` date DEFAULT NULL,
  `parity` tinyint(3) unsigned DEFAULT NULL,
  `wday` tinyint(3) unsigned DEFAULT NULL,
  `wdaynum` tinyint(3) unsigned DEFAULT NULL,
  KEY `pk_rule` (`rule_id`)
) ENGINE=InnoDB AUTO_INCREMENT=318 DEFAULT CHARSET=utf8;

/*!40000 ALTER TABLE `rule` ENABLE KEYS */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
