Feature: Testing taxonomy CRUD.

  @api @restful
  Scenario: Testing CRUD actions for Taxonomy.
    Given I "create" a term as "john" with the settings:
      | label | vocab             |
      | Water | science_personal1 |
     When I "patch" a term as "john" with the settings:
      | label       |
      | Water - new |
     Then I "delete" a term as "john" with the settings:
      | id    |
      | PREV  |

  @api @restful
  Scenario: Testing CRUD actions for taxonomy.
    Given I "create" a vocabulary as "john" with the settings:
      | label   | vsite | machine name  |
      | Testing | john  | testing_vocab |
     When I "patch" a vocabulary as "john" with the settings:
      | label         |
      | Testing - new |
     Then I "delete" a vocabulary as "john" with the settings:
      | id    |
      | PREV  |

  @api @restful
  Scenario: Testing creation of OG vocab.
    Given I "create" a vocabulary as "john" with the settings:
      | label   | vsite | machine name  |
      | Testing | john  | testing_vocab |
      And I "create" OG vocabulary as "john" with the settings:
      | entity type   | bundle  | vocabulary    |
      | node          | bio     | testing_vocab |
      And I am logging in as "john"
      And I visit "john/cp/build/taxonomy"
      And I should see "Testing"
     When I "patch" OG vocabulary as "john" with the settings:
      | bundle  |
      | blog    |
     Then I "delete" OG vocabulary as "john" with the settings:
      | id    |
      | PREV  |
