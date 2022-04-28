Feature:
  Check endpoints.

  @api @restful
  Scenario: Testing the layout endpoint
    Given I test the exposed resources:
    """
    api
    api/biblio
    api/bio
    api/blog
    api/book
    api/class
    api/class_material
    api/cv
    api/event
    api/faq
    api/feed
    api/media_gallery
    api/news
    api/page
    api/person
    api/presentation
    api/slideshow_slide
    api/software_project
    api/software_release
    api/taxonomy?vsite=2
    api/vocabulary?vsite=2
    """
