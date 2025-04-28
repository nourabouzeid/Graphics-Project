from PIL import Image
    
def create_single_color_image(color, size=(256, 256)):
    """Create a solid color image."""
    img = Image.new("RGBA", size, color)
    return img

def create_checkerboard_image(size, tile_size, color1, color2):
    """Create a checkerboard pattern image."""
    img = Image.new("RGBA", size)
    pixels = img.load()
    
    for y in range(size[1]):
        for x in range(size[0]):
            # Determine which tile we're in
            tile_x = x // tile_size[0]
            tile_y = y // tile_size[1]
            
            # Alternate colors based on tile position
            if (tile_x + tile_y) % 2 == 0:
                pixels[x, y] = color1
            else:
                pixels[x, y] = color2
                
    return img

# Create the images as described in the OpenGL code
white_img = create_single_color_image((255, 255, 255, 255))
black_img = create_single_color_image((0, 0, 0, 255))

checkerboard_albedo = create_checkerboard_image(
    size=(256, 256),
    tile_size=(128, 128),
    color1=(255, 255, 255, 255),
    color2=(16, 16, 16, 255)
)

checkerboard_specular = create_checkerboard_image(
    size=(256, 256),
    tile_size=(128, 128),
    color1=(0, 0, 0, 255),
    color2=(255, 255, 255, 255)
)
checkerboard_roughness = create_checkerboard_image(
    size=(256, 256),
    tile_size=(128, 128),
    color1=(255, 255, 255, 255),
    color2=(64, 64, 64, 255)
)

# Save the images
# white_img.save("white.png")
# black_img.save("black.png")
# checkerboard_albedo.save("checkerboard_albedo.png")
# checkerboard_specular.save("checkerboard_specular.png")
checkerboard_roughness.save("checkerboard_roughness.png")

print("Images created successfully!")