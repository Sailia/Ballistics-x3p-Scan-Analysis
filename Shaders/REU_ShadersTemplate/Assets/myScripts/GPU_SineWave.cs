using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GPU_SineWave : MonoBehaviour
{
	public Material mat;

    // Start is called before the first frame update
    void Start()
    {
        
    }

	private void OnRenderImage(RenderTexture source, RenderTexture destination)
	{
		// probably apply some kind of texture.SetPixels(pixels)
		Graphics.Blit(source, destination, mat);
	}


	// Update is called once per frame
	void Update()
    {
        
    }
}
