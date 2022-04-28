Feature:
  Creating nodes via rest.

  @api @restful
  Scenario: Creating biblio via rest.
    Given I create a new node of "biblio" as "john" with the settings:
      | Label       | Body                  | vsite | type  |
      | Rest biblio | This is a test Biblio | john  | 101   |
     When I visit "john/publications/rest-biblio"
     Then I should see "Rest biblio"

  @api @restful
  Scenario: Creating blog via rest.
    Given I create a new node of "blog" as "john" with the settings:
      | Label     | Body                 | vsite |
      | Rest blog | This is a test blog  | john  |
     When I visit "john/blog/rest-blog"
     Then I should see "Rest blog"

  @api @restful
  Scenario: Creating book via rest.
    Given I create a new node of "book" as "john" with the settings:
      | Label     | Body                | vsite |
      | Rest book | This is a test book | john  |
     When I visit "john/book/rest-book"
     Then I should see "Rest book"

  @api @restful
  Scenario: Creating class via rest.
    Given I create a new node of "class" as "john" with the settings:
      | Label       | Body                  | vsite |
      | Rest class  | This is a test class  | john  |
     When I visit "john/classes/rest-class"
     Then I should see "Rest class"

  @api @restful
  Scenario: Creating class material via rest.
    Given I create a new node of "class_material" as "john" with the settings:
      | Label               | Body                           | vsite | Parent     |
      | Rest class material | This is a test class material  | john  | Rest class |
     When I visit "john/classes/rest-class/materials/rest-class-material"
     Then I should see "Rest class material"

  @api @restful
  Scenario: Creating faq via rest.
     Given I create a new node of "faq" as "john" with the settings:
      | Label     | Body                | vsite  |
      | Rest FAQ  | This is a test FAQ  | john   |
      When I visit "john/faq/rest-faq"
      Then I should see "Rest FAQ"

  @api @restful
  Scenario: Creating event via rest.
    Given I create a new node of "event" as "john" with the settings:
      | Label       | Body                  | vsite  | Start date |
      | Rest event  | This is a test event  | john   | 1/1/2010   |
    When I visit "john/john/event/rest-event"
    Then I should see "Rest event"

  @api @restful
  Scenario: Creating feed via rest.
     Given I create a new node of "feed" as "john" with the settings:
      | Label     | Body                | vsite  | Field url      |
      | Rest feed | This is a test feed | john   | http://foo.bar |

  @api @restful
  Scenario: Creating gallery via rest.
     Given I create a new node of "image_gallery" as "john" with the settings:
      | Label         | vsite  | columns  | rows  | files               |
      | Rest gallery  | john   | 3        | 4     | jfk_1.jpg,jfk_2.jpg |
      When I visit "john/galleries/rest-gallery"
      Then I should see "Rest gallery"

  @api @restful
  Scenario: Creating news via rest.
     Given I create a new node of "news" as "john" with the settings:
      | Label     | date      | body                | vsite  |
      | Rest news | 1/1/2012  | This is a rest nest | john   |
      When I visit "john/galleries/rest-gallery"
      Then I should see "Rest gallery"

  @api @restful
  Scenario: Creating page via rest.
     Given I create a new node of "page" as "john" with the settings:
      | Label     | body                | vsite  | path       |
      | Rest page | This is a rest page | john   | rest-path  |
      When I visit "john/rest-page"
      Then I should see "Rest page"

  @api @restful
  Scenario: Creating person via rest.
     Given I create a new node of "person" as "john" with the settings:
      | address      | email            | first_name  | middle_name | last_name | phone     | prefix  | professional_title  | vsite   |
      | Rest address | foo@example.com  | Diego       | dela        | vega      | 555-1212  | Don.    | Zoro!               | john    |
      When I visit "john/people/diego-dela-vega"
      Then I should see "Zoro!"

  @api @restful
  Scenario: Creating faq via rest.
     Given I create a new node of "presentation" as "john" with the settings:
      | Label             | vsite | date     | location        |
      | Rest presentation | john  | 1/1/2012 | Home sweet home |

  #todo: handle files.
  @api @restful
  Scenario: Creating slideshow slide via rest.
     Given I create a new node of "slideshow_slide" as "john" with the settings:
      | Label           | vsite |
      | Rest slide show | john  |

  @api @restful
  Scenario: Creating software project via rest.
     Given I create a new node of "software_project" as "john" with the settings:
      | Label                 | vsite | files               |
      | Rest software project | john  | jfk_1.jpg,jfk_2.jpg |

  @api @restful
  Scenario: Creating software release via rest.
    # @TODO bring back package to "true"
     Given I create a new node of "software_release" as "john" with the settings:
      | Label                 | vsite | software_project      | body  | recommended | version |
      | Rest software release | john  | Rest software project | foo   | 1           | 2005    |
