import bpy

#esse algoritmo só converte um modelo glb por vez
#a biblioteca pra converter gltf tambem é usada aqui, igual nos códigos em C++

# 1. Caminho do arquivo .glb de entrada e do .txt de saída
caminho_glb = r"D:\PROJETOS\JIFIM\projeto pires\modelos_pessoas\person_0.glb" #voce precisa mudar o arquivo glb pro modelo que voce quiser
caminho_txt = r"D:\PROJETOS\JIFIM\projeto pires\convertidos\juntas_extraidas.txt" #o nome do arquivo convertido tambem muda

# Limpa a cena atual do Blender (opcional)
bpy.ops.wm.read_factory_settings(use_empty=True)

# 2. Importa o arquivo GLB para o Blender
bpy.ops.import_scene.gltf(filepath=caminho_glb)

# 3. Localiza o esqueleto (Armature) importado
esqueleto = None
for obj in bpy.context.scene.objects:
    if obj.type == 'ARMATURE':
        esqueleto = obj
        break

# 4. Extrai e salva as posições X, Y, Z no arquivo TXT
with open(caminho_txt, "w") as f:
    if esqueleto:
        # Se o modelo tiver estrutura de ossos (Bones)
        for bone in esqueleto.pose.bones:
            # Posição global do osso no espaço 3D
            pos = esqueleto.matrix_world @ bone.head
            f.write(f"{pos.x:.6f} {pos.y:.6f} {pos.z:.6f}\n")
    else:
        # Se o modelo usar apenas nós (Nodes/Empty objects)
        for obj in bpy.context.scene.objects:
            pos = obj.matrix_world.translation
            f.write(f"{pos.x:.6f} {pos.y:.6f} {pos.z:.6f}\n")

print(f"Juntas extraídas com sucesso e salvas em: {caminho_txt}")