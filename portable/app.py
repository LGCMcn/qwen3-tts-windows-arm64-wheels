import argparse
import os
from pathlib import Path

os.environ.update(HF_HUB_OFFLINE="1", TRANSFORMERS_OFFLINE="1",
                  GRADIO_ANALYTICS_ENABLED="False")
import numpy as np
import soundfile as sf
import torch
from qwen_tts import Qwen3TTSModel

ROOT = Path(__file__).resolve().parent
MODEL = ROOT / "models" / "Qwen3-TTS-12Hz-0.6B-CustomVoice"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--text", help="Text to synthesize without opening the UI")
    parser.add_argument("--speaker", default="Vivian")
    parser.add_argument("--language", default="Chinese")
    parser.add_argument("--output", type=Path, default=ROOT / "output.wav")
    args = parser.parse_args()

    if not torch.cuda.is_available():
        raise RuntimeError("CUDA unavailable. Check the NVIDIA RTX Spark driver.")
    print("Loading Qwen3-TTS on", torch.cuda.get_device_name(0), flush=True)
    model = Qwen3TTSModel.from_pretrained(
        str(MODEL), device_map="cuda:0", dtype=torch.bfloat16,
        attn_implementation="sdpa", local_files_only=True,
    )

    def synthesize(text, speaker, language):
        text = text.strip()
        if not text:
            raise ValueError("Enter text to synthesize.")
        wavs, rate = model.generate_custom_voice(
            text=text, speaker=speaker, language=language,
        )
        audio = np.asarray(wavs[0], dtype=np.float32)
        if audio.size == 0 or not np.isfinite(audio).all():
            raise RuntimeError("The model did not return valid audio.")
        return rate, audio

    if args.text is not None:
        rate, audio = synthesize(args.text, args.speaker, args.language)
        sf.write(args.output, audio, rate)
        print(sf.info(args.output))
        return

    import gradio as gr

    app = gr.Interface(
        fn=synthesize,
        inputs=[
            gr.Textbox(label="Text / 配音文本", lines=5),
            gr.Dropdown(choices=model.get_supported_speakers(), value="vivian", label="Speaker / 音色"),
            gr.Dropdown(choices=model.get_supported_languages(), value="chinese", label="Language / 语言"),
        ],
        outputs=gr.Audio(label="Audio / 配音", format="wav"),
        title="Qwen3-TTS 12Hz 0.6B CustomVoice",
    )
    app.queue(default_concurrency_limit=1).launch(
        server_name="127.0.0.1", share=False, inbrowser=True,
    )


if __name__ == "__main__":
    main()

