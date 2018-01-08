# Migration extra drush commands usage

> This is a procedural narrative about how to use migration extra drush commands.
Here is how to use the migration extra drush commands :

### Installation

```bash
cd drupal/docroot/
composer require "yosymfony/resource-watcher: 1.2.0"
cd drupal/docroot/modules/
git clone git@github.com:ihebattia/migrate_extra.git
drush en migrate_extra -y
```

### Developing migrations editing YAML files

> In this case you can either use ```drush migrate-debug-watch``` or ```drush migrate-debug-cycle```.

##### Procedures:

1. From your docroot run ```drush migrate-debug-watch```

This command watches the migration config files and will automatically import
new config any time you edit yaml and then rerun the target import.

Example:

Run ```drush migrate-debug-cycle dcaf_migration upgrade_d6_node_page --dependencies='upgrade_d6_user'```

2. From your docroot run ```drush migrate-debug-cycle```

This command is like ```drush migrate-debug-watch``` runs all the cycle of migration,
but doesn't watch your configuration files you will need to run it every time you change them.

Example:

Run ```drush migrate-debug-cycle dcaf_migration upgrade_d6_node_page --dependencies='upgrade_d6_user'```
