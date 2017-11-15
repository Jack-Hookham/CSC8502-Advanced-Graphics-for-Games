#include "../../nclgl/window.h"
#include "Renderer.h"

#pragma comment(lib, "nclgl.lib")

int main()
{
	Window w("Scene Graphs!", 1600, 900, false);
	if (!w.HasInitialised())
	{
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised())
	{
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	renderer.setScene(SceneID::SPACE);

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
	{
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	return 0;
}