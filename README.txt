/* $Id$ */

SUMMARY

Demo Site is a simple module that allows you to take a snapshot of a Drupal
demonstration site. It turns a Drupal installation into a sandbox that you can
use for testing modules or publicly demonstrating a module / extension / theme
(you name it). In short: With cron enabled, the Drupal site will be reset to
the dumped state in a definable interval. Of course you can reset the site
manually, too.


REQUIREMENTS

Cron, or alternatively Poormanscron http://drupal.org/project/poormanscron


INSTALLATION

* Copy the demo module to your modules directory and enable it on the Modules
  page (admin/build/modules).

* Optionally configure who is allowed to administer demo module, create dumps
  and reset the site at the Access control page (admin/user/access).

* Configure the path where dumps will be stored at the Dump settings
  (admin/settings/demo).

* Go to Create snapshot (admin/settings/demo/dump) and create your first
  snapshot.

* After a while, reset your site (admin/settings/demo/reset).


CONFIGURING AUTOMATIC RESET

* Go to Manage snapshots (admin/settings/demo/manage) and select a snapshot
  for cron.

* Enable atomatic reset from Dump settings (admin/settings/demo). Make sure you
  have cron configured to run at least once within the entered time interval.


CONTACT

For bug reports, feature suggestions and latest developments visit the project 
page: http://drupal.org/project/demo.

Authors:
Daniel F. Kudwien <dfk@unleashedmind.com>
Stefan Kudwien <smk@unleashedmind.com>

This module has been developed by UNLEASHED MIND - www.unleashedmind.com

UNLEASHED MIND is specialized on consulting and development of Drupal powered
websites, and offers services from hosting to customization to get you started.
