# directx11-engine
Simple 3d engine on DirectX11

Features:
 - Own mesh format (.mesh), terrain format(.terrain). For mesh created addon for Blender, for terrain was written python
script, which converts bitmap to .terrain. 
 - Support of loading dds textures (from DirectXTex library), loading vertex and pixel shaders.
 - Input system based on DirectInput.
 - Text rendering is performed by DirectWrite + in game console;
 - Directional ligthing; 
 - Normal mapping.
 - Timer

 # Blender_exporter
Installation:
 - Copy io_scene_mesh directory to <BlenderPath>\<BlenderVersion>\scripts\addons
 - Enable addon in Blender User Preferences -> Add-ons -> Import-Export -> MESH EXPORT
 
 To export use File -> Export -> To Mesh command.

 # Terrain_generator
 Replace picture.png by your heightmap and run run.bat. Output file: out.terrain

 # Screenshots
 
![](https://github.com/xotonic/directx11_game/blob/master/screenshot.PNG)
