Feature:
  Testing vsite related permissions.

# Authenticated user
  @api @vsite
  Scenario: Testing authenticated user can't access the control panel
    Given I am logging in as "demo"
     When I go to "cp/build/features"
     Then I should get a "403" HTTP response

  @api @vsite
  Scenario: Testing authenticated user can't access unpublished content
    Given I am logging in as "michelle"
     When I go to "john/blog/unpublish-blog"
     Then I should get a "403" HTTP response

# Vsite Member
  @api @vsite
  Scenario: Testing vsite member can't create a node outside of the vsite context.
    Given I am logging in as "michelle"
     When I go to "node/add"
     Then I should get a "403" HTTP response

  @api @vsite
  Scenario: Testing vsite member can create content only in the vsite they are member of.
    Given I am logging in as "alexander"
     When I go to "edison/node/add/blog"
      And I should get a "200" HTTP response
      And I go to "john/node/add/blog"
     Then I should get a "403" HTTP response

  @api @vsite
  Scenario: Testing vsite member can delete his own content.
    Given I am logging in as "michelle"
     When I delete the node of type "blog" named "Michelle's Blog"
      And I visit "obama/blog"
     Then I should not see "Welcome to Michelle's blog"

# Vsite admin
  @api @vsite
  Scenario: Testing vsite owner can assign a "vsite admin" role to a user.
    Given I am logging in as "john"
     When I give the user "bruce" the role "vsite admin" in the group "john"
      And I visit "john/cp/users"
     Then I should verify that the user "bruce" has a role of "vsite admin" in the group "john"

  @api @vsite
  Scenario: Testing vsite admin can view unpublished content.
    Given I am logging in as "bruce"
     When I go to "john/blog/unpublish-blog"
     Then I should get a "200" HTTP response

  @api @vsite
  Scenario: Testing edit ability for a vsite admin on won site.
    Given I am logging in as "alexander"
     When I visit "edison/publications"
     Then I should see the link "Links" under "ctools-dropdown-link-wrapper"

  @api @vsite
  Scenario: Testing edit inability for vsite admin on a different vsite.
    Given I am logging in as "alexander"
     When I visit "john/publications"
     Then I should not see the link "Links" under "ctools-dropdown-link-wrapper"

# content editor
  @api @vsite
  Scenario: Testing content editor can edit any content on his group.
    Given I am logging in as "john"
     When I give the user "klark" the role "content editor" in the group "john"
      And I logout
      And I am logging in as "klark"
      And I edit the node "First blog"
     Then I should get a "200" HTTP response
