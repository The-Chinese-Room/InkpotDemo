LIST Colour = White, Red, Blue, Green
VAR Cube1 = White

-> Main

== Main
    {  
    - BigCubeEncounter == 0 : 
        Welcome to Inkpot & Ink 
        This is a demo of some of types of interaction between Ink script and world
        Walk towards to the big cube
    - FlowsDemo.BabblingBlueBox == 0 and FlowsDemo.RamblingRedRoundel == 0 :
        Now check out the blue box or the red roundel 
    - FlowsDemo.BabblingBlueBox == 0:
        Check out the blue box
    - FlowsDemo.RamblingRedRoundel == 0 :
        Check out the red roundel 
    - FlowsDemo.BabblingBlueBox <= 2 and FlowsDemo.RamblingRedRoundel <= 0 :
        The flow in for red and blue will pause for each when the flow is switched
        Walk between the two to see this in action 
    - ParameterPassingDemo == 0 :
        Walk over to the parameter passing pad 
        And see how easy it is to pass parameters to Ink
    - else :
        Demo over :)
        Though you can still wander into the areas and see the flows switch
    }
    -> DONE

== BigCubeEncounter
    What colour should the cube be?
    +[White]
        ~Cube1 = White
    +[Red]
        ~Cube1 = Red
    +[Blue]
        ~Cube1 =Blue
    +[Green]
        ~Cube1 = Green
    - The cube is now {Cube1}
    Now go check out some of the other areas 
    -> DONE

== FlowsDemo( colour ) 
    {
    - colour == Red : 
        -> RamblingRedRoundel
    - else :
        -> BabblingBlueBox
    }
    -> DONE

= BabblingBlueBox
    Blue is the color of the sky and the ocean, two of nature's most vast and awe-inspiring wonders. 
    Blue is also one of the most popular colors in the world, beloved by people of all ages and cultures.
    Blue is a primary color in the RGB color model, which means that it cannot be created by mixing other colors. 
    Blue is also one of the three primary colors in the traditional RYB color model.
    Blue has a wide range of shades and hues, from light and airy sky blue to deep and rich navy blue. 
    Blue is often associated with positive emotions such as peace, tranquility, trust, and intelligence. 
    Blue is also associated with coldness, distance, and sadness.
    In many cultures, blue is associated with royalty and nobility. 
    Blue is also a popular color for corporate branding, as it conveys a sense of professionalism and trust. 
    The first documented use of blue pigment was in ancient Egypt, where it was used to decorate tombs and temples.
    In the Middle Ages, blue was a very expensive color, as it was made from the mineral lapis lazuli.
    The first synthetic blue dye was invented in 1828.
    Blue is the most popular color in the world, according to surveys.
    Blue is a color of the human eye.
    Blue is the color of the sky because of the way sunlight interacts with the atmosphere.
    Blue is a beautiful and versatile color that can be used in many different ways. 
    Blue is a popular choice for everything from home decor to fashion to art.
    -> DONE
    
= RamblingRedRoundel
    Red is the color at the long wavelength end of the visible spectrum of light, next to orange and opposite violet. 
    Red has a dominant wavelength of approximately 625–740 nanometres. 
    Red is a primary color in the RGB color model 
    Red is a secondary color (made from magenta and yellow) in the CMYK color model, and is the complementary color of cyan. 
    Reds range from the brilliant yellow-tinged scarlet and vermillion to bluish-red crimson. 
    Reds vary in shade from the pale red pink to the dark red burgundy.
    Red is one of the most powerful and evocative colors in the world. 
    Red is often associated with strong emotions such as love, passion, anger, and danger. 
    Red is also associated with heat, energy, and excitement.
    Red is a very visible color, which is why it is often used in warning signs and traffic lights. 
    Red is also a popular color for marketing and advertising, as it can grab people's attention quickly.
    Red is the color of blood, which is why it is often associated with danger and violence.
    Red is the color of fire, which is why it is often associated with heat and energy.
    Red is the color of love and passion, which is why it is often used in romantic settings.
    Red is the color of anger and rage, which is why it is often used to express these emotions.
    Red is a versatile color that can be used in many different ways. 
    Red is a popular choice for everything from home decor to fashion to art.
    -> DONE

== ParameterPassingDemo( ABoolean, TheAnswer, PI, Message, Nephews ) 
    Parameters passed are 
    ABoolean : { ABoolean }
    TheAnswer : { TheAnswer }
    PI : { PI }
    Message : { Message }
    Nephews : { Nephews }
    ->DONE
    


